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

struct Summary {
  [[nodiscard]] nlohmann::json toJson() const;

  std::string name;
  std::string time_text;
  std::int64_t total_msec;
};

inline nlohmann::json Summary::toJson() const {
  return {
      {"name", name},
      {"time_text", time_text},
      {"total_msec", total_msec},
  };
}

struct SummaryArray {
  [[nodiscard]] nlohmann::json toJson() const;

  std::vector<Summary> array;
};

inline nlohmann::json SummaryArray::toJson() const {
  nlohmann::json::array_t j;
  for (const auto& i : array) {
    j.push_back(std::move(i.toJson()));
  }
  return j;
}

struct Day {
  [[nodiscard]] nlohmann::json toJson() const;

  std::int64_t total_msec;
  std::string time_text;
};

inline nlohmann::json Day::toJson() const {
  return {{"total_msec", total_msec}, {"time_text", time_text}};
}

struct Total {
  [[nodiscard]] nlohmann::json toJson() const;

  std::string end;
  std::string start;
  std::string time_text;
  std::int64_t total_msec;
};

inline nlohmann::json Total::toJson() const {
  return {
      {"end", end},
      {"start", start},
      {"time_text", time_text},
      {"total_msec", total_msec},
  };
}

struct Result {
  [[nodiscard]] std::string toJson() const;

  SummaryArray categories;
  std::vector<Day> days;
  SummaryArray editors;
  SummaryArray languages;
  SummaryArray oss;
  SummaryArray projects;
  Total total;
};

inline std::string Result::toJson() const {
  nlohmann::json::array_t days_json;
  for (const auto& day : days) {
    days_json.push_back(day.toJson());
  }

  nlohmann::json j = {{"categories", categories.toJson()},  //
                      {"editors", editors.toJson()},        //
                      {"languages", languages.toJson()},    //
                      {"oss", oss.toJson()},                //
                      {"projects", projects.toJson()},      //
                      {"total", total.toJson()},            //
                      {"days", days_json}};                 //
  return j.dump();
}

}  // namespace waka::dto::summary::get

#endif  // WAKA_SRC_DTO_SUMMARY_GET_HPP_
