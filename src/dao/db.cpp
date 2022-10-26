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

#include "db.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <cassert>
#include <exception/sql_error.hpp>
#include <filesystem>

using fmt::format;
using std::shared_ptr;
using std::string;
using std::filesystem::exists;
using std::filesystem::is_directory;
using std::filesystem::perms;
using std::filesystem::status;
using waka::exception::DBError;
using waka::exception::SQLError;

namespace waka::dao {

shared_ptr<DB> DB::instance_;

// 检查waka-server的数据目录是否满足以下条件
// 1. 存在
// 2. 必须是目录
// 3. 用户对该目录拥有rwx权限
// 如果没有问题，返回空字符串；否则返回错误信息
[[nodiscard]] static string checkDataDir(const string& data_dir) noexcept {
  assert(!data_dir.empty());

  try {
    if (!exists(data_dir)) {
      return format("{} doesn't exist", data_dir);
    }
    if (!is_directory(data_dir)) {
      return format("{} isn't a dir", data_dir);
    }
    auto perms = status(data_dir).permissions();
    if ((perms & perms::owner_all) != perms::owner_all) {
      return format("no rwx permission on {}", data_dir);
    }
    return {};
  } catch (const std::exception& e) {
    return format("waka::checkDataDir e.what={}", e.what());
  }
}

void DB::open(const string& data_dir) {
  assert(sqlite3_ == nullptr);
  assert(!data_dir.empty());

  string what = checkDataDir(data_dir);
  if (!what.empty()) {
    throw DBError(std::move(what));
  }

  string db_path = data_dir + "/sqlite3.db";
  int ret = sqlite3_open(db_path.c_str(), &sqlite3_);
  if (ret) {
    string what =
        format("can't open or create sqlite3 database on {}", db_path);
    throw DBError(std::move(what));
  }
}

void DB::query(const string& sql, Callback cb, void* arg) const {
  assert(sqlite3_ != nullptr);
  assert(!(cb == nullptr) ^ (arg == nullptr));

  SPDLOG_DEBUG("DB:query sql='{}'", sql);

  char* errmsg = nullptr;
  int ret = sqlite3_exec(sqlite3_, sql.c_str(), cb, arg, &errmsg);
  if (ret) {
    string reason{errmsg};
    sqlite3_free(errmsg);
    throw SQLError(reason, sql);
  }
}

bool DB::exists(const string& data_dir) {
  assert(!data_dir.empty());

  string db_path = data_dir + "/sqlite3.db";
  return std::filesystem::exists(db_path);
}

}  // namespace waka::dao
