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

#include "msg.hpp"

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
}

static string parsePutConfigBody(const string& body, Param& param) {
  try {
    param = Param::fromJson(body);
  } catch (const JsonError& e) {
    return jsonMsg(e.what());
  }

  if (param.port < 1 || param.port > 65535) {
    return jsonMsg("port must be in the range of 1~65535");
  }
  if (param.timeout < 1 || param.timeout > Config::kMaxTimeout) {
    string error =
        format("timeout must be in the range of 1~{}", Config::kMaxTimeout);
    return jsonMsg(std::move(error));
  }
  if (!isValidIP(param.ip)) {
    string error = format("invalid ip address '{}'", param.ip);
    return jsonMsg(std::move(error));
  }
  return "";
}

void putConfig(const Request& req, Response& resp) {
  Param param;
  string msg = parsePutConfigBody(req.body, param);
  if (!msg.empty()) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(msg, "application/json");
  }

  Config config;
  config.setIP(param.ip);
  config.setLogLevel(param.log_level);
  config.setTimeFormat(param.time_format);
  config.setPort(param.port);
  config.setTimeout(param.timeout);
  MetaService{}.storeConfig(config);

  resp.status = HttpStatus::kOK;
  resp.set_content(jsonMsg("ok"), "application/json");
}

}  // namespace waka::controller
