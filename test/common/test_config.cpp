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

#include <common/config.hpp>
#include <dao/config_mapper.hpp>
#include <model/config.hpp>
#include <string>
#include <test.hpp>
#include <unordered_map>

using std::string;
using std::unordered_map;
using waka::dao::ConfigMapper;
using waka::test::setupDB;

namespace waka::common {

static unordered_map<string, string> kConfigOptions = {
    {"version", "1.0.0"},           //
    {"timeout", "135"},             //
    {"time_format", "%H小时%M分"},  //
    {"port", "8080"},               //
    {"log_level", "info"},          //
    {"ip", "127.0.0.1"},            //
};

static void initConfigTable() {
  setupDB();
  ConfigMapper mapper;
  mapper.createTable();
  for (const auto& [k, v] : kConfigOptions) {
    mapper.insert({k, v});
  }
}

TEST(TestConfig, TestStore) {
  initConfigTable();

  ConfigMapper mapper;

  Config config;
  config.setVersion("0.0.0");
  config.setIP("192.168.1.2");
  config.setPort(1234);
  config.setTimeFormat("%HH:%MM");
  config.setLogLevel(spdlog::level::off);
  config.setTimeout(15);
  config.store();

  model::Config c{"version", "0.0.0"};
  auto ret = mapper.get("version");
  EXPECT_EQ(ret.key(), c.key());
  EXPECT_EQ(ret.value(), c.value());

  c = {"ip", "192.168.1.2"};
  ret = mapper.get("ip");
  EXPECT_EQ(ret.key(), c.key());
  EXPECT_EQ(ret.value(), c.value());

  c = {"port", "1234"};
  ret = mapper.get("port");
  EXPECT_EQ(ret.key(), c.key());
  EXPECT_EQ(ret.value(), c.value());

  c = {"time_format", "%HH:%MM"};
  ret = mapper.get("time_format");
  EXPECT_EQ(ret.key(), c.key());
  EXPECT_EQ(ret.value(), c.value());

  c = {"log_level", "off"};
  ret = mapper.get("log_level");
  EXPECT_EQ(ret.key(), c.key());
  EXPECT_EQ(ret.value(), c.value());

  c = {"timeout", "15"};
  ret = mapper.get("timeout");
  EXPECT_EQ(ret.key(), c.key());
  EXPECT_EQ(ret.value(), c.value());
}

}  // namespace waka::common
