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

#include "status_bar.hpp"

#include <spdlog/spdlog.h>

#include <common/pattern.hpp>
#include <dto/status_bar/get.hpp>

using std::int64_t;
using waka::common::formatTime;
using waka::dto::statusbar::get::Result;
using waka::http::Request;
using waka::http::Response;
using waka::http::Status;

namespace waka::controller {

void StatusBarController::get(const Request& req, Response& resp) {
  SPDLOG_DEBUG("GET /api/users/current/statusbar/today");
  int64_t msec = heartbeat_service_.today();
  Result result(formatTime(msec));
  resp.setStatus(Status::kOK);
  resp.setContent(result.toJSON().dump(), "application/json");
  SPDLOG_INFO("GET /api/users/current/statusbar/today 200");
}

}  // namespace waka::controller
