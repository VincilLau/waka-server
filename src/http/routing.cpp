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

#include "routing.hpp"

#include <controller/config.hpp>
#include <controller/heartbeat.hpp>
#include <controller/index.hpp>
#include <controller/status_bar.hpp>
#include <controller/summary.hpp>
#include <define.hpp>

using httplib::Server;
using std::string;
using waka::controller::ConfigController;
using waka::controller::HeartbeatController;
using waka::controller::IndexController;
using waka::controller::StatusBarController;
using waka::controller::SummaryController;

namespace waka::http {

void setupRouting(Server& server) {
  server.Get("/", Wrapper<IndexController, Method::kGet>{});
  server.Get("/api/config", Wrapper<ConfigController, Method::kGet>{});
  server.Put("/api/config", Wrapper<ConfigController, Method::kPut>{});
  server.Post("/api/users/current/heartbeats.bulk",
              Wrapper<HeartbeatController, Method::kPost>{});
  server.Get("/api/users/current/statusbar/today",
             Wrapper<StatusBarController, Method::kGet>{});
  server.Get("/api/summary", Wrapper<SummaryController, Method::kGet>{});

  server.set_mount_point("/dashboard", "./assets/dashboard");
  server.set_mount_point("/css", "./assets/css");
  server.set_mount_point("/icon", "./assets/icon");
  server.set_mount_point("/js", "./assets/js");
}

}  // namespace waka::http
