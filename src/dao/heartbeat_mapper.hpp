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

#ifndef WAKA_SRC_DAO_HEARTBEAT_MAPPER_HPP_
#define WAKA_SRC_DAO_HEARTBEAT_MAPPER_HPP_

#include <sqlite3.h>

#include <cassert>
#include <model/heartbeat.hpp>
#include <set>

#include "db.hpp"

namespace waka::dao {

class HeartbeatMapper {
 public:
  HeartbeatMapper() : db_(getDB()) { assert(db_); }

  // 查询数据库中所有的heartbeat表，存储到tables_中
  void loadTables() const;

  // 在数据库中插入一条心跳
  // 该函数根据heartbeat的时间自动选择插入的表
  void insert(const model::Heartbeat& heartbeat) const;

 private:
  void createTable(const std::string& name) const;

  static std::set<std::string> tables_;

  sqlite3* db_;
};

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_HEARTBEAT_MAPPER_HPP_
