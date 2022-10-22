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

#ifndef WAKA_SRC_DTO_STATUS_BAR_GET_HPP_
#define WAKA_SRC_DTO_STATUS_BAR_GET_HPP_

#include <nlohmann/json.hpp>
#include <string>

namespace waka::dto::statusbar::get {

struct Result {
  Result(std::string time_text) : text(std::move(time_text)) {}
  std::string toJson() const;

  std::string text;
};

inline std::string Result::toJson() const {
  nlohmann::json j = {{"data", {{"grand_total", {{"text", text}}}}}};
  return j.dump();
}

}  // namespace waka::dto::statusbar::get

#endif  // WAKA_SRC_DTO_STATUS_BAR_GET_HPP_
