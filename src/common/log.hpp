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

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>

#include <common/config.hpp>
#include <define.hpp>
#include <filesystem>
#include <string>

namespace waka::common {

[[nodiscard]] inline static std::string logLevelToString(int level) {
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

[[nodiscard]] inline static int stringToLogLevel(const std::string& str) {
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

inline static void setupLogger() {
  auto log_path = std::filesystem::path{WAKA_DATA_DIR} / "log" / "waka.log";
  auto logger = spdlog::daily_logger_mt("waka_logger", log_path, 0, 0);
  spdlog::set_default_logger(logger);
  auto level = stringToLogLevel(Config::get().logLevel());
  spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %t [%@] -- %v");
}

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_LOG_HPP_
