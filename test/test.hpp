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

#ifndef WAKA_TEST_TEST_HPP_
#define WAKA_TEST_TEST_HPP_

#include <fmt/core.h>
#include <sqlite3.h>

#include <exception/db_error.hpp>
#include <dao/db.hpp>
#include <filesystem>

namespace waka::test {

static const char* kDBPath = "./waka.db";

inline static void setupDB() {
  std::filesystem::path db_path{kDBPath};
  remove(db_path);
  sqlite3* db = nullptr;
  int ret = sqlite3_open(kDBPath, &db);
  if (ret) {
    throw exception::DBError(
        fmt::format("can't open sqlite3 database '{}'", kDBPath));
  }
  dao::setDB(db);
}

}  // namespace waka::test

#endif  // WAKA_TEST_TEST_HPP_
