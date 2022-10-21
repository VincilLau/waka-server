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

#include <dao/config_mapper.hpp>
#include <test.hpp>
#include <unordered_map>

using std::string;
using std::unordered_map;
using waka::test::setupDB;

namespace waka::dao {

TEST(TestConfig, TestCreateTable) {
  setupDB();

  ConfigMapper mapper;
  mapper.createTable();
}

static unordered_map<string, string> kConfigOptions = {
    {"version", "1.0.0"},           //
    {"timeout", "135"},             //
    {"time_format", "%H小时%M分"},  //
    {"port", "8080"},               //
    {"log_level", "info"},          //
    {"ip", "127.0.0.1"},            //
};

TEST(TestConfig, TestUpdate) {
  setupDB();

  ConfigMapper mapper;
  mapper.createTable();

  for (const auto& [k, v] : kConfigOptions) {
    mapper.insert({k, v});
  }

  auto config = mapper.get("ip");
  EXPECT_EQ(config.key(), "ip");
  EXPECT_EQ(config.value(), "127.0.0.1");
  mapper.update({"ip", "0.0.0.0"});
  config = mapper.get("ip");
  EXPECT_EQ(config.key(), "ip");
  EXPECT_EQ(config.value(), "0.0.0.0");
}

TEST(TestConfig, TestListAll) {
  setupDB();

  ConfigMapper mapper;
  mapper.createTable();

  for (const auto& [k, v] : kConfigOptions) {
    mapper.insert({k, v});
  }

  auto configs = mapper.listAll();
  EXPECT_EQ(configs.size(), kConfigOptions.size());
  EXPECT_EQ(configs[0].key(), "ip");
  EXPECT_EQ(configs[0].value(), "127.0.0.1");
  EXPECT_EQ(configs[1].key(), "log_level");
  EXPECT_EQ(configs[1].value(), "info");
  EXPECT_EQ(configs[2].key(), "port");
  EXPECT_EQ(configs[2].value(), "8080");
  EXPECT_EQ(configs[3].key(), "time_format");
  EXPECT_EQ(configs[3].value(), "%H小时%M分");
  EXPECT_EQ(configs[4].key(), "timeout");
  EXPECT_EQ(configs[4].value(), "135");
  EXPECT_EQ(configs[5].key(), "version");
  EXPECT_EQ(configs[5].value(), "1.0.0");
}

}  // namespace waka::dao
