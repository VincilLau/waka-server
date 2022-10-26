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

#include <common/date.hpp>
#include <model/heartbeat.hpp>
#include <set>
#include <vector>

#include "db.hpp"

namespace waka::dao {

// 查询heartbeat表的mapper
// heartbeat表按月分表，这对于HeartbeatMapper的使用者是透明的
class HeartbeatMapper {
 public:
  HeartbeatMapper() : db_(DB::getInstance()) {}

  // 查询数据库中所有的heartbeat表，存储到tables_中
  // 在服务器启动时调用
  void loadTableSet() const;
  // 在表中插入一条心跳
  void insert(const model::Heartbeat& heartbeat) const;
  // 列出某一日期的所有心跳
  // 按时间从小到大排序
  [[nodiscard]] std::vector<model::Heartbeat> listByDate(
      const common::Date& date) const;

 private:
  // 创建表名为name的heartbeat表
  void createTable(const std::string& name) const;
  // 判断是否存在表名为name的表
  [[nodiscard]] bool hasTable(const std::string& name) const {
    return table_set_.find(name) != table_set_.end();
  }
  // 所有heartbeat表，按表名字典序排序
  static std::set<std::string> table_set_;

  std::shared_ptr<DB> db_;
};

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_HEARTBEAT_MAPPER_HPP_
