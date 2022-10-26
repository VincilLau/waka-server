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

#ifndef WAKA_SRC_DTO_HEARTBEAT_POST_HPP_
#define WAKA_SRC_DTO_HEARTBEAT_POST_HPP_

#include <fmt/core.h>

#include <exception/json_error.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace waka::dto::heartbeat::post {

struct Heartbeat {
  static void checkJSON(const nlohmann::json& j);

  std::shared_ptr<std::string> branch;
  std::shared_ptr<std::string> category;
  std::shared_ptr<int> cursorpos;
  std::shared_ptr<std::vector<std::string>> dependencies;
  std::shared_ptr<std::string> entity;
  std::shared_ptr<bool> is_write;
  std::shared_ptr<std::string> language;
  std::shared_ptr<int> lineno;
  std::shared_ptr<int> lines;
  std::shared_ptr<std::string> project;
  std::shared_ptr<double> time;
  std::shared_ptr<std::string> type;
  std::shared_ptr<std::string> user_agent;
};

struct Param {
  static Param fromJSON(const std::string& json_str);

  std::vector<Heartbeat> heartbeats;
};

struct Result {
  // 创建具有n个响应的Result对象
  Result(std::size_t n) : responses(n) {}

  nlohmann::json toJSON() const;

  std::vector<std::pair<std::string, int>> responses;
};

}  // namespace waka::dto::heartbeat::post

#endif  // WAKA_SRC_DTO_HEARTBEAT_POST_HPP_
