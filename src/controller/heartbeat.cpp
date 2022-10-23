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

#include "error.hpp"

using httplib::Request;
using httplib::Response;
using waka::bo::Heartbeat;
using waka::common::HttpStatus;
using waka::dto::heartbeat::Param;
using waka::dto::heartbeat::Result;
using waka::exception::JsonError;
using waka::service::HeartbeatService;

namespace waka::controller {

void postHeartbeat(const Request& req, Response& resp) {
  Param param;
  try {
    param = Param::fromJson(req.body);
  } catch (const JsonError& e) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(getMsgJson(e.what()), "application/json");
    return;
  }

  Result result(param.heartbeats.size());
  HeartbeatService service;
  for (size_t i = 0; i < param.heartbeats.size(); i++) {
    auto& h = param.heartbeats[i];
    if (!h.entity || !h.time) {
      result.responses[i].second = HttpStatus::kBadRequest;
      continue;
    }

    Heartbeat heartbeat;
    heartbeat.entity = std::move(*h.entity);
    heartbeat.time = static_cast<int64_t>(*h.time * 1000);
    if (h.branch) {
      heartbeat.branch = std::move(*h.branch);
    }
    if (h.category) {
      heartbeat.category = std::move(*h.category);
    }
    if (h.language) {
      heartbeat.language = std::move(*h.language);
    }
    if (h.project) {
      heartbeat.project = std::move(*h.project);
    }
    if (h.type) {
      heartbeat.type = std::move(*h.type);
    }

    try {
      result.responses[i].first = service.save(heartbeat);
    } catch (const std::exception& e) {
      resp.status = HttpStatus::kInternalServerError;
      resp.set_content(getMsgJson(e.what()), "application/json");
      return;
    }
    result.responses[i].second = HttpStatus::kCreated;
  }

  try {
    resp.status = HttpStatus::kAccepted;
    resp.set_content(result.toJson(), "application/json");
  } catch (const std::exception& e) {
    resp.status = HttpStatus::kInternalServerError;
    resp.set_content(getMsgJson(e.what()), "application/json");
  }
}

}  // namespace waka::controller
