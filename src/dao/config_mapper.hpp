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

#ifndef WAKA_SRC_DAO_CONFIG_MAPPER_HPP_
#define WAKA_SRC_DAO_CONFIG_MAPPER_HPP_

#include <sqlite3.h>

#include <model/config.hpp>
#include <vector>

#include "db.hpp"

namespace waka::dao {

class ConfigMapper {
 public:
  explicit ConfigMapper() : db_(getDB()) {}

  // 创建config表
  void createTable() const;
  // 插入一条配置项
  void insert(const model::Config& config) const;
  // 更新配置项
  void update(const model::Config& config) const;
  // 查找key对应的配置项
  model::Config get(const std::string& key) const;
  // 列出所有配置项，按key的字典序排序
  std::vector<model::Config> listAll() const;

 private:
  sqlite3* db_;
};

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_CONFIG_MAPPER_HPP_
