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

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <common/uuid.hpp>
#include <dao/db.hpp>
#include <exception/db_error.hpp>
#include <filesystem>
#include <fstream>

using std::ofstream;
using std::size_t;
using std::string;
using std::filesystem::create_directories;
using std::filesystem::path;
using std::filesystem::perm_options;
using std::filesystem::perms;
using std::filesystem::temp_directory_path;
using waka::common::genUUIDv4;
using waka::exception::DBError;

namespace waka::dao {

TEST(DBTest, testOpenDBSuccess) {
  path data_dir =
      path{temp_directory_path()} / "waka-server-test" / genUUIDv4();
  create_directories(data_dir);

  DB db;
  db.open(data_dir.string());
}

TEST(DBTest, testOpenDBDataDirNotExists) {
  path data_dir =
      path{temp_directory_path()} / "waka-server-test" / genUUIDv4();

  DB db;
  EXPECT_THROW(db.open(data_dir.string()), DBError);
}

TEST(DBTest, testOpenDBDataDirNotDir) {
  path data_dir =
      path{temp_directory_path()} / "waka-server-test" / genUUIDv4();
  ofstream ofs(data_dir);
  ASSERT_TRUE(ofs.is_open());
  ofs.close();

  DB db;
  EXPECT_THROW(db.open(data_dir.string()), DBError);
}

TEST(DBTest, testOpenDBDataDirNoPerms) {
  path data_dir =
      path{temp_directory_path()} / "waka-server-test" / genUUIDv4();
  create_directories(data_dir);
  permissions(data_dir, perms::none, perm_options::replace);

  DB db;
  EXPECT_THROW(db.open(data_dir.string()), DBError);
}

}  // namespace waka::dao

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %t [%@] -- %v");
  return RUN_ALL_TESTS();
}
