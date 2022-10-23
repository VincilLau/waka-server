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

#include <common/http.hpp>
#include <common/pattern.hpp>
#include <dto/status_bar/get.hpp>
#include <service/heartbeat_service.hpp>

#include "error.hpp"

using httplib::Request;
using httplib::Response;
using waka::common::formatTime;
using waka::common::HttpStatus;
using waka::dto::statusbar::get::Result;
using waka::service::HeartbeatService;

namespace waka::controller {

void getStatusBar(const Request& req, Response& resp) {
  try {
    int64_t msec = HeartbeatService{}.today();
    int64_t hour = msec / 1000 / 3600;
    int64_t min = msec / 1000 / 60 % 60;
    Result result(formatTime(hour, min));
    resp.status = HttpStatus::kOK;
    resp.set_content(result.toJson(), "application/json");
  } catch (const std::exception& e) {
    resp.status = HttpStatus::kInternalServerError;
    resp.set_content(getMsgJson(e.what()), "application/json");
  }
}

}  // namespace waka::controller
