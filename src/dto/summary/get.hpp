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

#ifndef WAKA_SRC_DTO_SUMMARY_GET_HPP_
#define WAKA_SRC_DTO_SUMMARY_GET_HPP_

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace waka::dto::summary::get {

struct Item {
  [[nodiscard]] nlohmann::json toJSON() const {
    return {
        {"name", name},
        {"time_text", time_text},
        {"total_msec", total_msec},
    };
  }

  std::string name;
  std::string time_text;
  std::int64_t total_msec;
};

struct ItemArray {
  [[nodiscard]] nlohmann::json toJSON() const {
    nlohmann::json::array_t j;
    for (const auto& i : array) {
      j.push_back(i.toJSON());
    }
    return j;
  }

  std::vector<Item> array;
};

struct Day {
  [[nodiscard]] nlohmann::json toJSON() const {
    return {
        {"total_msec", total_msec},  //
        {"time_text", time_text},    //
    };
  }

  std::int64_t total_msec;
  std::string time_text;
};

struct Total {
  [[nodiscard]] nlohmann::json toJSON() const {
    return {
        {"end", end},
        {"start", start},
        {"time_text", time_text},
        {"total_msec", total_msec},
    };
  }

  std::string end;
  std::string start;
  std::string time_text;
  std::int64_t total_msec;
};

struct Result {
  [[nodiscard]] nlohmann::json toJSON() const;

  std::vector<Day> days;
  ItemArray editors;
  ItemArray languages;
  ItemArray oss;
  ItemArray projects;
  Total total;
};

inline nlohmann::json Result::toJSON() const {
  nlohmann::json::array_t days_json;
  for (const auto& d : days) {
    days_json.push_back(d.toJSON());
  }

  nlohmann::json j = {
      {"editors", editors.toJSON()},      //
      {"languages", languages.toJSON()},  //
      {"oss", oss.toJSON()},              //
      {"projects", projects.toJSON()},    //
      {"total", total.toJSON()},          //
      {"days", std::move(days_json)},     //
  };
  return j.dump();
}

}  // namespace waka::dto::summary::get

#endif  // WAKA_SRC_DTO_SUMMARY_GET_HPP_
