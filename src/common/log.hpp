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

#ifndef WAKA_SRC_COMMON_LOG_HPP_
#define WAKA_SRC_COMMON_LOG_HPP_

#include <spdlog/spdlog.h>

#include <string>

namespace waka::common {

static std::string logLevelToString(int level) {
  switch (level) {
    case spdlog::level::trace:
      return "trace";
    case spdlog::level::debug:
      return "debug";
    case spdlog::level::info:
      return "info";
    case spdlog::level::warn:
      return "warn";
    case spdlog::level::err:
      return "error";
    case spdlog::level::critical:
      return "critical";
    case spdlog::level::off:
      return "off";
  }
  return "";
}

static int stringToLogLevel(const std::string& str) {
  if (str == "trace") {
    return spdlog::level::trace;
  } else if (str == "debug") {
    return spdlog::level::debug;
  } else if (str == "info") {
    return spdlog::level::info;
  } else if (str == "warn") {
    return spdlog::level::warn;
  } else if (str == "error") {
    return spdlog::level::err;
  } else if (str == "critical") {
    return spdlog::level::critical;
  } else if (str == "off") {
    return spdlog::level::off;
  }
  return -1;
}

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_LOG_HPP_
