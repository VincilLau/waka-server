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

#include <controller/config.hpp>
#include <dao/db.hpp>
#include <exception/db_error.hpp>
#include <filesystem>
#include <service/meta_service.hpp>

using fmt::format;
using httplib::Server;
using std::exception;
using std::filesystem::exists;
using std::filesystem::path;
using waka::common::Config;
using waka::controller::getConfig;
using waka::controller::putConfig;
using waka::dao::setDB;
using waka::exception::DBError;
using waka::service::MetaService;

using namespace std;

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
    MetaService service;
    service.init();
  }
}

static void setupRouting(Server& server) {
  server.Get("/api/config", getConfig);
  server.Put("/api/config", putConfig);
}

static void runServer(const string& ip, uint16_t port) {
  Server server;
  setupRouting(server);
  SPDLOG_INFO("listen on '{}:{}'", ip, port);
  server.listen(ip.c_str(), port);
}

int main() {
  try {
    setupLogger();
    setupDB();

    MetaService service;
    Config config = service.loadConfig();
    runServer(config.ip(), config.port());
  } catch (const exception& e) {
    SPDLOG_CRITICAL("{}", e.what());
  }

  return 0;
}
