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

#include "heartbeat_mapper.hpp"

#include <common/date.hpp>
#include <exception/sql_error.hpp>

using fmt::format;
using std::set;
using std::string;
using waka::common::Date;
using waka::exception::SqlError;
using waka::model::Heartbeat;

namespace waka::dao {

set<string> HeartbeatMapper::tables_;

static const char* kLoadTablesSql =
    "SELECT `name` FROM `sqlite_master` "
    "WHERE `type`='table' AND "
    "`name` LIKE 'heartbeat_'";

static int loadTablesCallback(void* set, int n, char** texts, char** names) {
  assert(set);
  assert(n == 1);
  assert(string{names[0]} == "name");

  auto tables = static_cast<std::set<string>*>(set);
  tables->insert(texts[0]);
  return 0;
}

void HeartbeatMapper::loadTables() const {
  char* errmsg = nullptr;
  int ret =
      sqlite3_exec(db_, kLoadTablesSql, loadTablesCallback, &tables_, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), kLoadTablesSql);
  }
}

static const char* kInsertSql =
    "INSERT INTO `{}` ("
    "branch,"
    "category,"
    "editor,"
    "entity,"
    "id,"
    "language,"
    "os,"
    "project,"
    "time,"
    "type"
    ") VALUES ("
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "{},"
    "'{}'"
    ")";

void HeartbeatMapper::insert(const Heartbeat& heartbeat) const {
  Date date = Date::fromUnixMilli(heartbeat.time * 1000);
  string table = format("heartbeat_{:04d}_{:02d}", date.year(), date.month());
  bool table_exists = tables_.find(table) != tables_.end();
  if (!table_exists) {
    createTable(table);
    tables_.insert(table);
  }

  string sql = format(kInsertSql, table,
                      heartbeat.branch,    //
                      heartbeat.category,  //
                      heartbeat.editor,    //
                      heartbeat.entity,    //
                      heartbeat.id,        //
                      heartbeat.language,  //
                      heartbeat.os,        //
                      heartbeat.project,   //
                      heartbeat.time,      //
                      heartbeat.type       //
  );
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
}

static const char* kCreateTableSql =
    "CREATE TABLE `{}` ("
    "`branch`   VARCHAR(255) NOT NULL,"
    "`category` VARCHAR(255) NOT NULL,"
    "`editor`   VARCHAR(255) NOT NULL,"
    "`entity`   TEXT         NOT NULL,"
    "`id`       CHAR(36)     NOT NULL,"
    "`language` VARCHAR(255) NOT NULL,"
    "`os`       VARCHAR(255) NOT NULL,"
    "`project`  VARCHAR(255) NOT NULL,"
    "`time`     BIGINT       NOT NULL,"
    "`type`     VARCHAR(255) NOT NULL"
    ")";

static const char* kCreateIndexSql =
    "CREATE INDEX `time_index` "
    "ON `{}` (`time`);";

void HeartbeatMapper::createTable(const string& name) const {
  string sql = format(kCreateTableSql, name);
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }

  sql = format(kCreateIndexSql, name);
  ret = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
}

}  // namespace waka::dao
