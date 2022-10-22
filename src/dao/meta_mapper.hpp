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

#ifndef WAKA_SRC_DAO_META_MAPPER_HPP_
#define WAKA_SRC_DAO_META_MAPPER_HPP_

#include <sqlite3.h>

#include <cassert>
#include <model/meta.hpp>
#include <vector>

#include "db.hpp"

namespace waka::dao {

class MetaMapper {
 public:
  explicit MetaMapper() : db_(getDB()) { assert(db_); }

  // 创建meta表
  void createTable() const;
  // 插入一条数据项
  void insert(const model::Meta& meta) const;
  // 更新一条数据项
  void update(const model::Meta& meta) const;
  // 查找key对应的value
  std::string get(const std::string& key) const;
  // 列出所有项，按key的字典序排序
  std::vector<model::Meta> listAll() const;

 private:
  sqlite3* db_;
};

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_META_MAPPER_HPP_
