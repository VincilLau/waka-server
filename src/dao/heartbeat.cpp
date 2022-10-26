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

#include "heartbeat.hpp"

#include <common/date.hpp>
#include <exception/sql_error.hpp>

using fmt::format;
using std::int64_t;
using std::set;
using std::string;
using std::vector;
using waka::common::Date;
using waka::model::Heartbeat;

namespace waka::dao {

set<string> HeartbeatMapper::table_set_;

static const char* kLoadTableSetSQL =
    "SELECT `name` FROM `sqlite_master` "
    "WHERE `type`='table' AND "
    "`name` LIKE 'heartbeat%'";

static int loadTableListCallback(void* table_set, int n, char** texts,
                                 char** names) {
  assert(table_set);
  assert(n == 1);
  assert(string{names[0]} == "name");

  auto tables = static_cast<std::set<string>*>(table_set);
  tables->insert(texts[0]);
  return 0;
}

void HeartbeatMapper::loadTableSet() const {
  db_->query(kLoadTableSetSQL, loadTableListCallback, &table_set_);
}

static const char* kCreateTableSQL =
    "CREATE TABLE `{}` ("
    "`branch`   VARCHAR(255)             NOT NULL,"
    "`editor`   VARCHAR(255)             NOT NULL,"
    "`entity`   TEXT                     NOT NULL,"
    "`id`       CHAR(36)     PRIMARY KEY NOT NULL,"
    "`language` VARCHAR(255)             NOT NULL,"
    "`os`       VARCHAR(255)             NOT NULL,"
    "`project`  VARCHAR(255)             NOT NULL,"
    "`time`     BIGINT                   NOT NULL"
    ")";

static const char* kCreateIndexSQL =
    "CREATE INDEX `time_index` "
    "ON `{}` (`time`);";

void HeartbeatMapper::createTable(const string& name) const {
  string sql = format(kCreateTableSQL, name);
  db_->query(sql, nullptr, nullptr);

  sql = format(kCreateIndexSQL, name);
  db_->query(sql, nullptr, nullptr);
}

static const char* kInsertSQL =
    "INSERT INTO `{}` ("
    "`branch`,"
    "`editor`,"
    "`entity`,"
    "`id`,"
    "`language`,"
    "`os`,"
    "`project`,"
    "`time`"
    ") VALUES ("
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "'{}',"
    "{}"
    ")";

void HeartbeatMapper::insert(const Heartbeat& heartbeat) const {
  Date date = Date::fromUnixMilli(heartbeat.time);
  string table = format("heartbeat_{:04d}_{:02d}", date.year(), date.month());
  if (!hasTable(table)) {
    createTable(table);
    table_set_.insert(table);
  }

  string sql = format(kInsertSQL, table,
                      heartbeat.branch,    //
                      heartbeat.editor,    //
                      heartbeat.entity,    //
                      heartbeat.id,        //
                      heartbeat.language,  //
                      heartbeat.os,        //
                      heartbeat.project,   //
                      heartbeat.time       //
  );
  db_->query(sql, nullptr, nullptr);
}

static const char* kListByDateSQL =
    "SELECT "
    "`branch`,"
    "`editor`,"
    "`entity`,"
    "`id`,"
    "`language`,"
    "`os`,"
    "`project`,"
    "`time`"
    " FROM `{}` "
    "WHERE `time` >= {} "
    "AND `time` < {} "
    "ORDER BY `time`";

static int listByDateCallback(void* lst_vector, int n, char** texts,
                              char** names) {
  assert(lst_vector);
  assert(n == 8);
  assert(string{names[0]} == "branch");
  assert(string{names[1]} == "editor");
  assert(string{names[2]} == "entity");
  assert(string{names[3]} == "id");
  assert(string{names[4]} == "language");
  assert(string{names[5]} == "os");
  assert(string{names[6]} == "project");
  assert(string{names[7]} == "time");

  auto lst = static_cast<vector<Heartbeat>*>(lst_vector);
  Heartbeat h;
  h.branch = texts[0];
  h.editor = texts[1];
  h.entity = texts[2];
  h.id = texts[3];
  h.language = texts[4];
  h.os = texts[5];
  h.project = texts[6];
  h.time = atoll(texts[7]);
  lst->push_back(std::move(h));

  return 0;
}

vector<Heartbeat> HeartbeatMapper::listByDate(const Date& date) const {
  Date today = Date::today();
  if (date > today) {
    return {};
  }

  string table = format("heartbeat_{:04d}_{:02d}", date.year(), date.month());
  if (!hasTable(table)) {
    return {};
  }

  Date next_day = date;
  next_day++;
  string sql =
      format(kListByDateSQL, table, date.unixMilli(), next_day.unixMilli());
  vector<Heartbeat> lst;
  db_->query(sql, listByDateCallback, &lst);
  return lst;
}

}  // namespace waka::dao
