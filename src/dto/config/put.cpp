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

#include "put.hpp"

#include <fmt/format.h>

#include <exception/json_error.hpp>
#include <nlohmann/json.hpp>

using fmt::format;
using nlohmann::json;
using std::string;
using waka::exception::JSONError;

namespace waka::dto::config::put {

Param Param::fromJSON(const string& json_str) {
  Param param;
  json j;
  try {
    j = json::parse(json_str);
  } catch (const json::parse_error& e) {
    throw JSONError(e.what());
  }

  if (j.is_null()) {
    throw JSONError("param can't be null");
  } else if (!j.is_object()) {
    throw JSONError(format("param must be a object, not {}", j.type_name()));
  }

  auto ip = j["ip"];
  auto log_level = j["log_level"];
  auto port = j["port"];
  auto timeout = j["timeout"];
  auto time_format = j["time_format"];

  if (ip.is_null()) {
    throw JSONError("'ip' can't be null");
  } else if (log_level.is_null()) {
    throw JSONError("'log_level' can't be null");
  } else if (port.is_null()) {
    throw JSONError("'port' can't be null");
  } else if (timeout.is_null()) {
    throw JSONError("'timeout' can't be null");
  } else if (time_format.is_null()) {
    throw JSONError("'time_format' can't be null");
  }

  if (!ip.is_string()) {
    throw JSONError(format("'ip' must be a string, not {}", j.type_name()));
  } else if (!log_level.is_string()) {
    throw JSONError(
        format("'log_level' must be a string, not {}", j.type_name()));
  } else if (!port.is_number_integer()) {
    throw JSONError(format("'port' must be a integer, not {}", j.type_name()));
  } else if (!timeout.is_number_integer()) {
    throw JSONError(
        format("'timeout' must be a integer, not {}", j.type_name()));
  } else if (!time_format.is_string()) {
    throw JSONError(
        format("'time_format' must be a string, not {}", j.type_name()));
  }

  param.ip = ip;
  param.log_level = log_level;
  param.port = port;
  param.timeout = timeout;
  param.time_format = time_format;

  return param;
}

}  // namespace waka::dto::config::put