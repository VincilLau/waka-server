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

#include "log.hpp"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <define.hpp>
#include <filesystem>

#if WAKA_PLATFORM == WAKA_PLATFORM_LINUX
// 导入isatty
#include <unistd.h>
#endif  // #if WAKA_PLATFORM == WAKA_PLATFORM_LINUX

using std::string;
using std::filesystem::path;

namespace waka::common {

void initLogger(const string& data_dir) {
  auto log_dir = path{data_dir} / "log" / "waka.log";
  auto logger = spdlog::daily_logger_mt("waka_logger", log_dir.string(), 0, 0);
  spdlog::set_default_logger(logger);

  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %t [%@] -- %v");

#if WAKA_PLATFORM == WAKA_PLATFORM_LINUX
  if (isatty(STDOUT_FILENO)) {
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdout_sink->set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] %t [%@] -- %v");
    logger->sinks().push_back(stdout_sink);
  }
#endif  // #if WAKA_PLATFORM == WAKA_PLATFORM_LINUX
}

const char* logLevelToStr(int level) {
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
  return {};
}

int strToLogLevel(const string& str) {
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
