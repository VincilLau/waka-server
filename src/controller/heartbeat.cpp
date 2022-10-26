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

#include "heartbeat.hpp"

#include <spdlog/spdlog.h>

#include "msg.hpp"

using fmt::format;
using std::size_t;
using std::string;
using waka::dto::heartbeat::post::Heartbeat;
using waka::dto::heartbeat::post::Param;
using waka::dto::heartbeat::post::Result;
using waka::exception::JSONError;
using waka::http::Request;
using waka::http::Response;
using waka::http::Status;

namespace waka::controller {

bool HeartbeatController::checkDto(const Heartbeat& dto, size_t index) {
  if (!dto.entity) {
    SPDLOG_WARN(
        "POST /api/users/current/heartbeats.bulk 400, "
        "index={}, msg='entity can't be null'",
        index);
    return false;
  }
  if (!dto.time) {
    SPDLOG_WARN(
        "POST /api/users/current/heartbeats.bulk 400, "
        "index={}, msg='time can't be null'",
        index);
    return false;
  }
  if (dto.entity->empty()) {
    SPDLOG_WARN(
        "POST /api/users/current/heartbeats.bulk 400, "
        "index={}, msg='entity can't be empty'",
        index);
    return false;
  }
  return true;
}

bo::Heartbeat HeartbeatController::dtoToBO(const Heartbeat& dto) {
  bo::Heartbeat bo;
  bo.entity = std::move(*dto.entity);
  bo.time = static_cast<int64_t>(*dto.time * 1000);
  if (dto.branch) {
    bo.branch = std::move(*dto.branch);
  }
  if (dto.language) {
    bo.language = std::move(*dto.language);
  }
  if (dto.project) {
    bo.project = std::move(*dto.project);
  }
  if (dto.user_agent) {
    bo.user_agent = std::move(*dto.user_agent);
  }
  return bo;
}

void HeartbeatController::post(const Request& req, Response& resp) {
  SPDLOG_DEBUG("POST /api/users/current/heartbeats.bulk");

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
    SPDLOG_WARN(
        "POST /api/users/current/heartbeats.bulk 400, msg='{}', data='{}'",
        e.what(), req.getBody());
    resp.setStatus(Status::kBadRequest);
    resp.setContent(jsonMsg(e.what()), "application/json");
    return;
  }

  Result result = saveHeartbeats(param);

  resp.setStatus(Status::kAccepted);
  resp.setContent(result.toJSON().dump(), "application/json");
  SPDLOG_INFO("POST /api/users/current/heartbeats.bulk 202");
}

Result HeartbeatController::saveHeartbeats(const Param& param) const {
  Result result(param.heartbeats.size());
  for (size_t i = 0; i < param.heartbeats.size(); i++) {
    auto& dto = param.heartbeats[i];
    bool ok = checkDto(dto, i);
    if (!ok) {
      result.responses[i].second = Status::kBadRequest;
      continue;
    }

    bo::Heartbeat bo = dtoToBO(dto);
    try {
      result.responses[i].first = heartbeat_service_.save(bo);
    } catch (const std::exception& e) {
      SPDLOG_WARN(
          "POST /api/users/current/heartbeats.bulk 500, index={}, msg='{}'", i,
          e.what());
      continue;
    }
    result.responses[i].second = Status::kCreated;
    SPDLOG_INFO("POST /api/users/current/heartbeats.bulk 201, index={}", i);
  }
  return result;
}

}  // namespace waka::controller
