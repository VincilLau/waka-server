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

#ifndef WAKA_SRC_DTO_CONFIG_PUT_HPP_
#define WAKA_SRC_DTO_CONFIG_PUT_HPP_

#include <exception/json_error.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace waka::dto::config::put {

struct Param {
 public:
  static Param fromJson(const std::string& json);

  std::string ip;
  std::string log_level;
  int port;
  int timeout;
  std::string time_format;
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
  } else if (!j.is_object()) {
    throw exception::JsonError("param must be a object");
  }

  auto ip = j["ip"];
  auto log_level = j["log_level"];
  auto port = j["port"];
  auto timeout = j["timeout"];
  auto time_format = j["time_format"];

  if (ip.is_null()) {
    throw exception::JsonError("'ip' can't be null");
  } else if (log_level.is_null()) {
    throw exception::JsonError("'log_level' can't be null");
  } else if (port.is_null()) {
    throw exception::JsonError("'port' can't be null");
  } else if (timeout.is_null()) {
    throw exception::JsonError("'timeout' can't be null");
  } else if (time_format.is_null()) {
    throw exception::JsonError("'time_format' can't be null");
  }

  if (!ip.is_string()) {
    throw exception::JsonError("'ip' must be a string");
  } else if (!log_level.is_string()) {
    throw exception::JsonError("'log_level' must be a string");
  } else if (!port.is_number_integer()) {
    throw exception::JsonError("'port' must be a integer");
  } else if (!timeout.is_number_integer()) {
    throw exception::JsonError("'timeout' must be a integer");
  } else if (!time_format.is_string()) {
    throw exception::JsonError("'time_format' must be a string");
  }

  param.ip = ip;
  param.log_level = log_level;
  param.port = port;
  param.timeout = timeout;
  param.time_format = time_format;

  return param;
}

}  // namespace waka::dto::config::put

#endif  // WAKA_SRC_DTO_CONFIG_PUT_HPP_
