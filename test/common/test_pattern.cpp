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

#include <common/pattern.hpp>

using std::string;

namespace waka::common {

TEST(PatternTest, testIsValidIP) {
  EXPECT_TRUE(isValidIP("127.0.0.1"));
  EXPECT_TRUE(isValidIP("0.0.0.0"));
  EXPECT_TRUE(isValidIP("192.168.3.55"));
  EXPECT_FALSE(isValidIP("192.168.1.1/24"));
  EXPECT_FALSE(isValidIP("192.168.256.3"));
  EXPECT_FALSE(isValidIP("192.168.255.-1"));
  EXPECT_FALSE(isValidIP("192.168.255"));
  EXPECT_FALSE(isValidIP("hello world"));
}

TEST(PatternTest, testParseUserAgent) {
  string ua =
      "wakatime/1.55.1 (linux-5.15.75-1-lts-unknown) go1.19.1 Unknown/0";
  string os;
  string editor;
  parseUserAgent(ua, os, editor);
  EXPECT_EQ(os, "Linux");
  EXPECT_EQ(editor, "Unknown");

  ua = "wakatime/1.55.2 (linux-5.10.102.1-microsoft-standard-WSL2-x86_64) "
       "go1.19.1 vscode/0.0.1";
  parseUserAgent(ua, os, editor);
  EXPECT_EQ(os, "WSL");
  EXPECT_EQ(editor, "VS Code");

  ua = "wakatime/1.55.2 (linux-5.10.102.1-microsoft-standard-WSL2-x86_64) "
       "go1.19.1 Idea/1.0.0";
  parseUserAgent(ua, os, editor);
  EXPECT_EQ(os, "WSL");
  EXPECT_EQ(editor, "Idea");

  ua = "wakatime/1.55.2 (linux-5.10.102.1-microsoft-standard-WSL2-x86_64) "
       "go1.19.1 PycharmCore/1.0.0";
  parseUserAgent(ua, os, editor);
  EXPECT_EQ(os, "WSL");
  EXPECT_EQ(editor, "PyCharm");
}

}  // namespace waka::common

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %t [%@] -- %v");
  return RUN_ALL_TESTS();
}
