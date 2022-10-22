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

using httplib::Server;
using waka::controller::getConfig;
using waka::dao::setDB;
using waka::exception::DBError;

using namespace std;

static const char* kDBPath = "./waka.db";

static void setupLogger() {
  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
}

static void setupDB() {
  sqlite3* db = nullptr;
  int ret = sqlite3_open(kDBPath, &db);
  if (ret) {
    throw DBError(fmt::format("can't open sqlite3 database '{}'", kDBPath));
  }
  setDB(db);
}

static void setupRouting(Server& server) {
  server.Get("/api/config", getConfig);
}

int main() {
  setupLogger();
  setupDB();

  Server server;
  setupRouting(server);
  server.listen("127.0.0.1", 8080);
  return 0;
}
