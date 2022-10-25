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

#ifndef WAKA_SRC_DAO_DB_HPP_
#define WAKA_SRC_DAO_DB_HPP_

#include <sqlite3.h>

#include <string>

namespace waka::dao {

// 数据库连接对象
using DB = sqlite3*;

// 打开DAO层访问的数据库
// data_dir为waka-server存储数据的目录，该目录必须已经存在
// 数据库的路径为${data_dir}/sqlite3.db
// 返回数据库连接对象
// 如果发生错误会抛出DBError异常
[[nodiscard]] DB openDB(const std::string& data_dir);
// 获取DAO层访问的数据库连接对象
[[nodiscard]] DB getDB() noexcept;
// 设置DAO层访问的数据库连接对象
void setDB(DB db) noexcept;

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_DB_HPP_
