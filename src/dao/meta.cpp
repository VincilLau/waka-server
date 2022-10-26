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

#include "meta.hpp"

#include <fmt/core.h>

#include <cassert>
#include <define.hpp>

using fmt::format;
using std::string;
using std::vector;
using waka::model::Meta;

namespace waka::dao {

static const char* kCreateTableSQL =
    "CREATE TABLE `meta` ("
    "`key` VERCHAR(255) PRIMARY KEY NOT NULL,"
    "`value` VERCHAR(255) NOT NULL"
    ")";

void MetaMapper::createTable() const {
  db_->query(kCreateTableSQL, nullptr, nullptr);
}

static const char* kInsertSQL =
    "INSERT INTO `meta`"
    "(`key`, `value`) VALUES "
    "('{}', '{}')";

void MetaMapper::insert(const Meta& meta) const {
  string sql = format(kInsertSQL, meta.key, meta.value);
  db_->query(sql, nullptr, nullptr);
}

static const char* kUpdateSQL =
    "UPDATE `meta` "
    "SET `value`='{}' "
    "WHERE `key`='{}'";

void MetaMapper::update(const Meta& meta) const {
  string sql = format(kUpdateSQL, meta.value, meta.key);
  db_->query(sql, nullptr, nullptr);
}

static const char* kGetSQL =
    "SELECT `value` "
    "FROM `meta` "
    "WHERE `key`='{}'";

static int getCallback(void* value_str, int n, char** texts, char** names) {
  assert(value_str != nullptr);
  assert(n == 1);
  assert(string{names[0]} == "value");

  string* value = static_cast<string*>(value_str);
  value->assign(texts[0]);
  return 0;
}

string MetaMapper::get(const string& key) const {
  string sql = format(kGetSQL, key);
  string value;
  db_->query(sql, getCallback, &value);
  return value;
}

static const char* kListAllSQL =
    "SELECT `key`, `value` FROM `meta` "
    "ORDER BY `key`";

static int listAllCallback(void* metas_vector, int n, char** texts,
                           char** names) {
  assert(metas_vector != nullptr);
  assert(n == 2);
  assert(string{names[0]} == "key");
  assert(string{names[1]} == "value");

  auto metas = static_cast<vector<Meta>*>(metas_vector);
  metas->push_back({texts[0], texts[1]});
  return 0;
}

vector<Meta> MetaMapper::listAll() const {
  vector<Meta> metas;
  db_->query(kListAllSQL, listAllCallback, &metas);
  return metas;
}

}  // namespace waka::dao
