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

#include "config_mapper.hpp"

#include <fmt/core.h>
#include <sqlite3.h>

#include <cassert>
#include <exception/sql_error.hpp>
#include <string>

using fmt::format;
using std::string;
using std::vector;
using waka::exception::SqlError;
using waka::model::Config;

namespace waka::dao {

static const char* kCreateTableSql =
    "CREATE TABLE `config` ("
    "`key` VERCHAR(255) PRIMARY KEY NOT NULL,"
    "`value` VERCHAR(255) NOT NULL"
    ")";

void ConfigMapper::createTable() const {
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, kCreateTableSql, nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), kCreateTableSql);
  }
}

static const char* kInsertSql =
    "INSERT INTO `config`"
    "(`key`, `value`) VALUES "
    "('{}', '{}')";

void ConfigMapper::insert(const Config& config) const {
  string sql = format(kInsertSql, config.key(), config.value());
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
}

static const char* kUpdateSql =
    "UPDATE `config` "
    "SET 'value'='{}' "
    "WHERE `key`='{}'";

void ConfigMapper::update(const Config& config) const {
  string sql = format(kUpdateSql, config.value(), config.key());
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
    "FROM `config` "
    "WHERE `key`='{}'";

static int getCallback(void* value, int n, char** texts, char** names) {
  assert(value);
  assert(n == 1);
  assert(string{names[0]} == "value");

  string* v = static_cast<string*>(value);
  v->assign(texts[0]);
  return 0;
}

Config ConfigMapper::get(const string& key) const {
  string sql = format(kGetSql, key);
  string value;

  vector<Config> configs;
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, sql.c_str(), getCallback, &value, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), std::move(sql));
  }
  return {key, value};
}

static const char* kListAllSql =
    "SELECT `key`, `value` FROM `config` "
    "ORDER BY `key`";

static int listAllCallback(void* configs, int n, char** texts, char** names) {
  assert(configs);
  assert(n == 2);
  assert(string{names[0]} == "key");
  assert(string{names[1]} == "value");

  auto v = static_cast<vector<Config>*>(configs);
  v->emplace_back(texts[0], texts[1]);
  return 0;
}

vector<Config> ConfigMapper::listAll() const {
  vector<Config> configs;
  char* errmsg = nullptr;
  int ret = sqlite3_exec(db_, kListAllSql, listAllCallback, &configs, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SqlError(std::move(reason), kListAllSql);
  }
  return configs;
}

}  // namespace waka::dao
