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

#include <model/meta.hpp>
#include <vector>

#include "db.hpp"

namespace waka::dao {

// 查询meta表的mapper
class MetaMapper {
 public:
  MetaMapper() : db_(DB::getInstance()) {}

  // 创建meta表
  void createTable() const;
  // 插入一条数据项
  void insert(const model::Meta& meta) const;
  // 更新一条数据项
  void update(const model::Meta& meta) const;
  // 查找key对应的value
  [[nodiscard]] std::string get(const std::string& key) const;
  // 列出所有数据项，按key的字典序排序
  [[nodiscard]] std::vector<model::Meta> listAll() const;

 private:
  std::shared_ptr<DB> db_;
};

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_META_MAPPER_HPP_
