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

#ifndef WAKA_SRC_CONTROLLER_HEARTBEAT_HPP_
#define WAKA_SRC_CONTROLLER_HEARTBEAT_HPP_

#include <bo/heartbeat.hpp>
#include <dto/heartbeat/post.hpp>
#include <http/http.hpp>
#include <service/heartbeat_service.hpp>

namespace waka::controller {

class HeartbeatController {
 public:
  // POST /api/users/current/heartbeats.bulk
  void post(const http::Request& req, http::Response& resp);

 private:
  [[nodiscard]] dto::heartbeat::post::Result saveHeartbeats(
      const dto::heartbeat::post::Param& param) const;

  [[nodiscard]] static bool checkDto(const dto::heartbeat::post::Heartbeat& dto,
                                     std::size_t index);
  static bo::Heartbeat dtoToBO(const dto::heartbeat::post::Heartbeat& dto);

 private:
  service::HeartbeatService heartbeat_service_;
};

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_HEARTBEAT_HPP_
