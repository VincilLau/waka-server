// Copyright 2022 Vincil Lau
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "config.hpp"

#include <spdlog/spdlog.h>

#include <cassert>
#include <common/config.hpp>
#include <common/http.hpp>
#include <common/log.hpp>
#include <dto/config/get.hpp>
#include <service/meta_service.hpp>
#include <stdexcept>

using httplib::Request;
using httplib::Response;
using std::exception;
using std::string;
using waka::common::Config;
using waka::common::HttpStatus;
using waka::common::logLevelToString;
using waka::dto::config::get::Result;
using waka::service::MetaService;

namespace waka::controller {

void getConfig(const Request& rep, Response& resp) {
  try {
    MetaService service;
    Config config = service.loadConfig();

    Result result;
    result.ip = config.ip();
    result.port = config.port();
    result.time_format = config.timeFormat();
    result.timeout = config.timeout();

    string logLevelStr = logLevelToString(config.logLevel());
    assert(!logLevelStr.empty());
    result.log_level = std::move(logLevelStr);

    resp.set_content(result.toJson(), "application/json");
  } catch (const exception& e) {
    string json = string{"{\"error\": \""} + e.what() + "\"}";
    SPDLOG_ERROR("{}", json);
    resp.status = HttpStatus::kInternalServerError;
    resp.set_content(json, "application/json");
  }
}

}  // namespace waka::controller
