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

#ifndef WAKA_SRC_CONTROLLER_ROUTING_HPP_
#define WAKA_SRC_CONTROLLER_ROUTING_HPP_

#include <define.hpp>
#include <filesystem>

#include "config.hpp"
#include "error.hpp"
#include "heartbeat.hpp"
#include "status_bar.hpp"
#include "summaries.hpp"

namespace waka::controller {

inline static void index(const httplib::Request& req, httplib::Response& resp) {
  resp.set_redirect("/dashboard");
}

inline static void mountStaticFile(httplib::Server& server) {
  server.set_file_request_handler(
      [](const httplib::Request& req, httplib::Response& resp) {
        // 打开keep-alive后响应缓慢
        resp.set_header("Connection", "close");
      });
  server.set_mount_point(
      "/css", std::filesystem::path{WAKA_PROJECT_DIR} / "assets" / "css");
  server.set_mount_point("/dashboard", std::filesystem::path{WAKA_PROJECT_DIR} /
                                           "assets" / "dashboard");
  server.set_mount_point(
      "/js", std::filesystem::path{WAKA_PROJECT_DIR} / "assets" / "js");
  server.set_mount_point(
      "/icon", std::filesystem::path{WAKA_PROJECT_DIR} / "assets" / "icon");
}

inline static void setErrorHandler(httplib::Server& server) {
  server.Get(".*", notFound)
      .Post(".*", notFound)
      .Put(".*", notFound)
      .Delete(".*", methodNotAllowed)
      .Options(".*", methodNotAllowed)
      .Patch(".*", methodNotAllowed)
      .set_exception_handler(exceptionHandler);
}

inline static void setupRouting(httplib::Server& server) {
  server.Get("/", index)
      .Post("/api/users/current/heartbeats.bulk", postHeartbeat)
      .Get("/api/users/current/statusbar/today", getStatusBar)
      .Get("/api/summaries", getSummaries)
      .Get("/api/config", getConfig)
      .Put("/api/config", putConfig);

  mountStaticFile(server);
  setErrorHandler(server);
}

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_ROUTING_HPP_
