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

#include "db.hpp"

#include <spdlog/spdlog.h>

#include <cassert>
#include <dao/heartbeat_mapper.hpp>
#include <define.hpp>
#include <exception/db_error.hpp>
#include <filesystem>
#include <service/meta_service.hpp>

using fmt::format;
using std::filesystem::create_directory;
using std::filesystem::exists;
using std::filesystem::path;
using waka::exception::DBError;
using waka::service::MetaService;

namespace waka::dao {

static sqlite3* DB;

void initDB() {
  if (!exists(WAKA_DATA_DIR)) {
    create_directory(WAKA_DATA_DIR);
  }

  path db_path = path{WAKA_DATA_DIR} / "sqlite3.db";
  bool db_exists = exists(db_path);

  sqlite3* db = nullptr;
  int ret = sqlite3_open(db_path.c_str(), &db);
  if (ret) {
    throw DBError(format("can't open sqlite3 database '{}'", db_path.string()));
  }
  DB = db;

  if (!db_exists) {
    SPDLOG_DEBUG("init meta table");
    MetaService{}.init();
  }

  HeartbeatMapper{}.loadTables();
}

sqlite3* getDB() {
  assert(DB);
  return DB;
}

}  // namespace waka::dao
