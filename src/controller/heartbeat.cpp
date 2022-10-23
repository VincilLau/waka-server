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

#include <bo/heartbeat.hpp>
#include <common/http.hpp>
#include <dao/heartbeat_mapper.hpp>
#include <dto/heartbeat/post.hpp>
#include <exception/json_error.hpp>
#include <service/heartbeat_service.hpp>

#include "msg.hpp"

using httplib::Request;
using httplib::Response;
using waka::bo::Heartbeat;
using waka::common::HttpStatus;
using waka::dto::heartbeat::post::Param;
using waka::dto::heartbeat::post::Result;
using waka::exception::JsonError;
using waka::service::HeartbeatService;

namespace waka::controller {

static bo::Heartbeat dtoToBO(const dto::heartbeat::post::Heartbeat& dto) {
  Heartbeat bo;
  bo.entity = std::move(*dto.entity);
  bo.time = static_cast<int64_t>(*dto.time * 1000);
  if (dto.branch) {
    bo.branch = std::move(*dto.branch);
  }
  if (dto.category) {
    bo.category = std::move(*dto.category);
  }
  if (dto.language) {
    bo.language = std::move(*dto.language);
  }
  if (dto.project) {
    bo.project = std::move(*dto.project);
  }
  if (dto.type) {
    bo.type = std::move(*dto.type);
  }
  if (dto.user_agent) {
    bo.user_agent = std::move(*dto.user_agent);
  }
  return bo;
}

void postHeartbeat(const Request& req, Response& resp) {
  Param param;
  try {
    param = Param::fromJson(req.body);
  } catch (const JsonError& e) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(jsonMsg(e.what()), "application/json");
    return;
  }

  Result result(param.heartbeats.size());
  HeartbeatService service;
  for (size_t i = 0; i < param.heartbeats.size(); i++) {
    auto& dto = param.heartbeats[i];
    if (!dto.entity || !dto.time) {
      result.responses[i].second = HttpStatus::kBadRequest;
      continue;
    }

    Heartbeat bo = dtoToBO(dto);
    try {
      result.responses[i].first = service.save(bo);
    } catch (const std::exception& e) {
      resp.status = HttpStatus::kInternalServerError;
      resp.set_content(jsonMsg(e.what()), "application/json");
      return;
    }
    result.responses[i].second = HttpStatus::kCreated;
  }

  resp.status = HttpStatus::kAccepted;
  resp.set_content(result.toJson(), "application/json");
}

}  // namespace waka::controller
