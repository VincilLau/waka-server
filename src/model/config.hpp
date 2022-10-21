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

#ifndef WAKA_SRC_MODEL_CONFIG_HPP_
#define WAKA_SRC_MODEL_CONFIG_HPP_

#include <sqlite3.h>

#include <string>
#include <unordered_map>

namespace waka::model {

// 配置表，存储waka-server的配置项
class Config {
 public:
  Config(std::string key, std::string value)
      : key_(std::move(key)), value_(std::move(value)) {}

  [[nodiscard]] const std::string& key() const { return key_; }
  [[nodiscard]] const std::string& value() const { return value_; }

 private:
  std::string key_;    // 配置项的键
  std::string value_;  // 配置项的值
};

}  // namespace waka::model

#endif  // WAKA_SRC_MODEL_CONFIG_HPP_
