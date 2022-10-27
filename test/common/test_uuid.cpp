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
#include <regex>

using std::regex;
using std::regex_match;
using std::string;

namespace waka::common {

static const regex kUUIDPattern{"^[0-9a-f]{8}(-[0-9a-f]{4}){3}-[0-9a-f]{12}$"};

TEST(UUIDTest, testGenUUIDv4) {
  string uuid = genUUIDv4();
  EXPECT_TRUE(regex_match(uuid, kUUIDPattern));
  uuid = genUUIDv4();
  EXPECT_TRUE(regex_match(uuid, kUUIDPattern));
  uuid = genUUIDv4();
  EXPECT_TRUE(regex_match(uuid, kUUIDPattern));
}

}  // namespace waka::common

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %t [%@] -- %v");
  return RUN_ALL_TESTS();
}
