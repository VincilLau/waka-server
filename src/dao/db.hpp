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

#include <cassert>
#include <memory>
#include <string>

namespace waka::dao {

// 表示一个SQLite3数据库连接
class DB {
 public:
  // 处理数据库查询结果的回调函数
  // 详见https://www.sqlite.org/c3ref/exec.html
  using Callback = int (*)(void*, int, char**, char**);

  // 构造一个表示已关闭的数据库连接对象
  DB() : sqlite3_(nullptr) {}
  // 关闭数据库并销毁DB对象
  ~DB() {
    if (sqlite3_ != nullptr) {
      close();
    }
  }

  // 打开数据库，如果数据库不存在，则创建数据库并初始化meta表
  // data_dir为waka-server存储数据的目录，该目录必须已经存在
  // 数据库的路径为${data_dir}/sqlite3.db
  // 如果发生错误会抛出DBError异常
  void open(const std::string& data_dir);
  // 关闭数据库连接
  void close() {
    assert(sqlite3_ != nullptr);
    sqlite3_close(sqlite3_);
  }
  // 如果数据库已关闭，返回true；否则返回false
  [[nodiscard]] bool closed() const { return sqlite3_ == nullptr; }

  // 查询数据库，若出错则抛出SQLError
  void query(const std::string& sql, Callback cb, void* arg) const;

  // 获取全局数据库连接实例
  [[nodiscard]] static std::shared_ptr<DB> getInstance() noexcept {
    assert(instance_ != nullptr);
    return instance_;
  }
  // 设置全局数据库连接实例
  static void setInstance(std::shared_ptr<DB> db) noexcept {
    assert(instance_ == nullptr);
    instance_ = db;
  }

 private:
  // 全局数据库连接实例
  static std::shared_ptr<DB> instance_;

  sqlite3* sqlite3_;
};

}  // namespace waka::dao

#endif  // WAKA_SRC_DAO_DB_HPP_
