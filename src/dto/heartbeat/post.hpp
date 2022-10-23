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
  static Param fromJson(const std::string& json);

  std::vector<Heartbeat> heartbeats;
};

inline Param Param::fromJson(const std::string& json) {
  Param param;
  nlohmann::json j;
  try {
    j = nlohmann::json::parse(json);
  } catch (const nlohmann::json::parse_error& e) {
    throw exception::JsonError(e.what());
  }

  if (j.is_null()) {
    throw exception::JsonError("param can't be null");
  } else if (!j.is_array()) {
    throw exception::JsonError("param must be a array");
  }

  for (size_t i = 0; i < j.size(); i++) {
    Heartbeat heartbeat;
    const auto& item = j[i];
    auto branch = item["branch"];
    if (!branch.is_null()) {
      if (!branch.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].branch' must be a string", i));
      }
      heartbeat.branch = std::make_shared<std::string>(std::move(branch));
    }

    auto category = item["category"];
    if (!category.is_null()) {
      if (!category.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].category' must be a string", i));
      }
      heartbeat.category = std::make_shared<std::string>(std::move(category));
    }

    auto cursorpos = item["cursorpos"];
    if (!cursorpos.is_null()) {
      if (!cursorpos.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].cursorpos' must be a integer", i));
      }
      heartbeat.cursorpos = std::make_shared<int>(cursorpos);
    }

    auto dependencies = item["dependencies"];
    if (!dependencies.is_null()) {
      if (!dependencies.is_array()) {
        throw exception::JsonError(
            fmt::format("'[{}].dependencies' must be a array", i));
      }
      heartbeat.dependencies = std::make_shared<std::vector<std::string>>();
      for (size_t j = 0; j < dependencies.size(); j++) {
        const auto& d = dependencies[i];
        if (!d.is_string()) {
          throw exception::JsonError(
              fmt::format("'[{}].dependencies[{}]' must be a array", i, j));
        }
        heartbeat.dependencies->push_back(std::move(d));
      }
    }

    auto entity = item["entity"];
    if (!entity.is_null()) {
      if (!entity.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].entity' must be a string", i));
      }
      heartbeat.entity = std::make_shared<std::string>(std::move(entity));
    }

    auto type = item["type"];
    if (!type.is_null()) {
      if (!type.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].type' must be a string", i));
      }
      heartbeat.type = std::make_shared<std::string>(std::move(type));
    }

    auto is_write = item["is_write"];
    if (!is_write.is_null()) {
      if (!is_write.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].is_write' must be a bool", i));
      }
      heartbeat.is_write = std::make_shared<bool>(is_write);
    }

    auto language = item["language"];
    if (!language.is_null()) {
      if (!language.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].language' must be a string", i));
      }
      heartbeat.language = std::make_shared<std::string>(std::move(language));
    }

    auto lineno = item["lineno"];
    if (!lineno.is_null()) {
      if (!lineno.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].lineno' must be a integer", i));
      }
      heartbeat.lineno = std::make_shared<int>(lineno);
    }

    auto lines = item["lines"];
    if (!lines.is_null()) {
      if (!lines.is_number_integer()) {
        throw exception::JsonError(
            fmt::format("'[{}].lines' must be a integer", i));
      }
      heartbeat.lines = std::make_shared<int>(std::move(lines));
    }

    auto project = item["project"];
    if (!project.is_null()) {
      if (!project.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].project' must be a string", i));
      }
      heartbeat.project = std::make_shared<std::string>(std::move(project));
    }

    auto time = item["time"];
    if (!time.is_null()) {
      if (!time.is_number_float()) {
        throw exception::JsonError(
            fmt::format("'[{}].time' must be a float number", i));
      }
      heartbeat.time = std::make_shared<double>(time);
    }

    auto user_agent = item["user_agent"];
    if (!user_agent.is_null()) {
      if (!user_agent.is_string()) {
        throw exception::JsonError(
            fmt::format("'[{}].user_agent' must be a string", i));
      }
      heartbeat.user_agent =
          std::make_shared<std::string>(std::move(user_agent));
    }

    param.heartbeats.push_back(std::move(heartbeat));
  }

  return param;
}

struct Result {
  Result(std::size_t n) : responses(n) {}

  std::string toJson() const;

  std::vector<std::pair<std::string, int>> responses;
};

inline std::string Result::toJson() const {
  nlohmann::json::array_t j;
  for (const auto& resp : responses) {
    nlohmann::json::array_t pair;
    pair.push_back({
        {"data", {{"id", resp.first}}},
    });
    pair.push_back(resp.second);
    j.push_back(std::move(pair));
  }
  return static_cast<nlohmann::json>(j).dump();
}

}  // namespace waka::dto::heartbeat::post

#endif  // WAKA_SRC_DTO_HEARTBEAT_POST_HPP_
