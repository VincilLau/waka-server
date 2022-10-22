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

#ifndef WAKA_SRC_DTO_CONFIG_GET_HPP_
#define WAKA_SRC_DTO_CONFIG_GET_HPP_

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

namespace waka::dto::config::get {

struct Result {
 public:
  std::string toJson() const;

  std::string ip;
  std::string log_level;
  std::uint16_t port;
  int timeout;
  std::string time_format;
};

inline std::string Result::toJson() const {
  nlohmann::json j = {
      {"ip", ip},                    //
      {"log_level", log_level},      //
      {"port", port},                //
      {"timeout", timeout},          //
      {"time_format", time_format},  //
  };
  return j.dump();
}

}  // namespace waka::dto::config::get

#endif  // WAKA_SRC_DTO_CONFIG_GET_HPP_
