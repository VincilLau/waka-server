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

#include "meta_mapper.hpp"

#include <fmt/core.h>
#include <sqlite3.h>

#include <cassert>
#include <exception/sql_error.hpp>
#include <string>

using fmt::format;
using std::string;
using std::vector;
using waka::exception::SqlError;
using waka::model::Meta;

namespace waka::dao {

static const char* kCreateTableSql =
    "CREATE TABLE `meta` ("
    "`key` VERCHAR(255) PRIMARY KEY NOT NULL,"
    "`value` VERCHAR(255) NOT NULL"
    ")";

void MetaMapper::createTable() const {
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, kCreateTableSql, nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), kCreateTableSql);
  }
}

static const char* kInsertSql =
    "INSERT INTO `meta`"
    "(`key`, `value`) VALUES "
    "('{}', '{}')";

void MetaMapper::insert(const Meta& meta) const {
  string sql = format(kInsertSql, meta.key(), meta.value());
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
}

static const char* kUpdateSql =
    "UPDATE `meta` "
    "SET `value`='{}' "
    "WHERE `key`='{}'";

void MetaMapper::update(const Meta& meta) const {
  string sql = format(kUpdateSql, meta.value(), meta.key());
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
}

static const char* kGetSql =
    "SELECT `value` "
    "FROM `meta` "
    "WHERE `key`='{}'";

static int getCallback(void* value, int n, char** texts, char** names) {
  assert(value);
  assert(n == 1);
  assert(string{names[0]} == "value");

  string* v = static_cast<string*>(value);
  v->assign(texts[0]);
  return 0;
}

string MetaMapper::get(const string& key) const {
  string sql = format(kGetSql, key);
  string value;
  vector<Meta> metas;
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), getCallback, &value, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
  return value;
}

static const char* kListAllSql =
    "SELECT `key`, `value` FROM `meta` "
    "ORDER BY `key`";

static int listAllCallback(void* metas, int n, char** texts, char** names) {
  assert(metas);
  assert(n == 2);
  assert(string{names[0]} == "key");
  assert(string{names[1]} == "value");

  auto v = static_cast<vector<Meta>*>(metas);
  v->emplace_back(texts[0], texts[1]);
  return 0;
}

vector<Meta> MetaMapper::listAll() const {
  vector<Meta> metas;
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, kListAllSql, listAllCallback, &metas, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), kListAllSql);
  }
  return metas;
}

}  // namespace waka::dao
