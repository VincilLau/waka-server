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

#include <httplib.h>
#include <spdlog/spdlog.h>
#include <sqlite3.h>

#include <common/config.hpp>
#include <common/http.hpp>
#include <controller/config.hpp>
#include <controller/error.hpp>
#include <controller/heartbeat.hpp>
#include <controller/status_bar.hpp>
#include <controller/summaries.hpp>
#include <dao/db.hpp>
#include <dao/heartbeat_mapper.hpp>
#include <exception/db_error.hpp>
#include <filesystem>
#include <service/heartbeat_service.hpp>
#include <service/meta_service.hpp>

using fmt::format;
using httplib::Server;
using std::exception;
using std::string;
using std::filesystem::exists;
using std::filesystem::path;
using waka::common::Config;
using waka::controller::exceptionHandler;
using waka::controller::getConfig;
using waka::controller::getStatusBar;
using waka::controller::getSummaries;
using waka::controller::methodNotAllowed;
using waka::controller::notFound;
using waka::controller::postHeartbeat;
using waka::controller::putConfig;
using waka::dao::HeartbeatMapper;
using waka::dao::setDB;
using waka::exception::DBError;
using waka::service::MetaService;

static void init();
static void runServer();

int main() {
  try {
    init();
    runServer();
  } catch (const exception& e) {
    SPDLOG_CRITICAL("{}", e.what());
    return 1;
  }
  return 0;
}

static const char* kDBPath = "./waka.db";

static void setupLogger() {
  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
}

static void setupDB() {
  path db_path{kDBPath};
  bool db_exists = exists(db_path);

  sqlite3* db = nullptr;
  int ret = sqlite3_open(kDBPath, &db);
  if (ret) {
    throw DBError(format("can't open sqlite3 database '{}'", kDBPath));
  }
  setDB(db);

  if (!db_exists) {
    SPDLOG_INFO("init meta table");
    MetaService{}.init();
  }

  HeartbeatMapper{}.loadTables();
}

static void setupConfig() {
  Config config = MetaService{}.loadConfig();
  Config::setConfig(std::move(config));
}

static void init() {
  setupLogger();
  setupDB();
  setupConfig();
}

static void setupRouting(Server& server) {
  server.Post("/api/users/current/heartbeats.bulk", postHeartbeat);
  server.Get("/api/users/current/statusbar/today", getStatusBar);
  server.Get("/api/summaries", getSummaries);
  server.Get("/api/config", getConfig);
  server.Put("/api/config", putConfig);

  server.Get(".*", notFound);
  server.Post(".*", notFound);
  server.Put(".*", notFound);

  server.Delete(".*", methodNotAllowed);
  server.Options(".*", methodNotAllowed);
  server.Patch(".*", methodNotAllowed);

  server.set_exception_handler(exceptionHandler);
}

void runServer() {
  Server server;
  setupRouting(server);

  const Config& config = Config::getConfig();
  SPDLOG_INFO("listen on '{}:{}'", config.ip(), config.port());
  server.listen(config.ip().c_str(), config.port());
}
