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

#include <dto/config/get.hpp>
#include <exception/json_error.hpp>
#include <exception/meta_data_error.hpp>

#include "msg.hpp"

using fmt::format;
using std::string;
using waka::common::MetaData;
using waka::dto::config::get::Result;
using waka::dto::config::put::Param;
using waka::exception::JSONError;
using waka::exception::MetaDataError;
using waka::http::Request;
using waka::http::Response;
using waka::http::Status;

namespace waka::controller {

void ConfigController::get(const Request& req, Response& resp) {
  SPDLOG_DEBUG("GET /api/config");

  MetaData meta_data = meta_service_.readMetaData();
  Result result;
  result.ip = std::move(meta_data.ip);
  result.port = meta_data.port;
  result.time_format = meta_data.time_format;
  result.timeout = meta_data.timeout;
  result.log_level = std::move(meta_data.log_level);

  resp.setStatus(Status::kOK);
  resp.setContent(result.toJSON().dump(), "application/json");
  SPDLOG_INFO("GET /api/config 200");
}

void ConfigController::put(const Request& req, Response& resp) {
  SPDLOG_DEBUG("PUT /api/config");

  string mime_type = req.getParam("Content-Type");
  if (mime_type != "application/json") {
    string msg = format("unexpected mime type {}", mime_type);
    SPDLOG_WARN("PUT /api/config -- {}", msg);
    resp.setStatus(Status::kBadRequest);
    resp.setContent(jsonMsg(msg), "application/json");
    return;
  }

  Param param;
  try {
    param = Param::fromJSON(req.getBody());
  } catch (const JSONError& e) {
    SPDLOG_WARN("PUT /api/config -- {}", e.what());
    resp.setStatus(Status::kBadRequest);
    resp.setContent(jsonMsg(e.what()), "application/json");
    return;
  }

  MetaData meta_data;
  meta_data.ip = std::move(param.ip);
  meta_data.log_level = std::move(param.log_level);
  meta_data.time_format = std::move(param.time_format);
  meta_data.port = param.port;
  meta_data.timeout = param.timeout;
  // 如果客户端发送的配置不合法，则会抛出MetaDataError异常
  try {
    meta_service_.writeMetaData(meta_data);
  } catch (const MetaDataError& e) {
    SPDLOG_WARN("PUT /api/config -- {}", e.what());
    resp.setStatus(Status::kBadRequest);
    resp.setContent(jsonMsg(e.what()), "application/json");
    return;
  }

  resp.setStatus(Status::kOK);
  resp.setContent(jsonMsg("ok"), "application/json");
  SPDLOG_INFO("PUT /api/config 200");
}

}  // namespace waka::controller
