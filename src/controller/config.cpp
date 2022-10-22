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
#include <common/pattern.hpp>
#include <dto/config/get.hpp>
#include <dto/config/put.hpp>
#include <exception/json_error.hpp>
#include <service/meta_service.hpp>
#include <stdexcept>

#include "error.hpp"

using fmt::format;
using httplib::Request;
using httplib::Response;
using std::string;
using waka::common::Config;
using waka::common::HttpStatus;
using waka::common::isValidIP;
using waka::dto::config::get::Result;
using waka::dto::config::put::Param;
using waka::exception::JsonError;
using waka::service::MetaService;

namespace waka::controller {

void getConfig(const Request& req, Response& resp) {
  try {
    Config config = MetaService{}.loadConfig();

    Result result;
    result.ip = config.ip();
    result.port = config.port();
    result.time_format = config.timeFormat();
    result.timeout = config.timeout();

    string logLevelStr = config.logLevel();
    assert(!logLevelStr.empty());
    result.log_level = std::move(logLevelStr);

    resp.status = HttpStatus::kOK;
    resp.set_content(result.toJson(), "application/json");
  } catch (const std::exception& e) {
    SPDLOG_ERROR("{}", e.what());
    resp.status = HttpStatus::kInternalServerError;
    resp.set_content(getMsgJson(e.what()), "application/json");
  }
}

void putConfig(const Request& req, Response& resp) {
  Param param;
  try {
    param = Param::fromJson(req.body);
  } catch (const JsonError& e) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(getMsgJson(e.what()), "application/json");
  }

  if (param.port < 1 || param.port > 65535) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(getMsgJson("port must be in the range of 1~65535"),
                     "application/json");
    return;
  }
  if (param.timeout < 1 || param.timeout > Config::kMaxTimeout) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(getMsgJson(format("port must be in the range of 1~{}",
                                       Config::kMaxTimeout)),
                     "application/json");
    return;
  }
  if (!isValidIP(param.ip)) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(getMsgJson(format("invalid ip address '{}'", param.ip)),
                     "application/json");
    return;
  }

  try {
    Config config;
    config.setIP(param.ip);
    config.setLogLevel(param.log_level);
    config.setTimeFormat(param.time_format);
    config.setPort(param.port);
    config.setTimeout(param.timeout);
    MetaService{}.storeConfig(config);

    resp.status = HttpStatus::kOK;
    resp.set_content(getMsgJson("ok"), "application/json");
    return;
  } catch (const std::exception& e) {
    SPDLOG_ERROR("{}", e.what());
    resp.status = HttpStatus::kInternalServerError;
    resp.set_content(getMsgJson(e.what()), "application/json");
  }
}

}  // namespace waka::controller
