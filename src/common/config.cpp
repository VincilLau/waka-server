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

#include "config.hpp"

#include <spdlog/spdlog.h>

#include <cassert>
#include <common/re.hpp>
#include <dao/config_mapper.hpp>

using std::string;
using std::to_string;
using waka::dao::ConfigMapper;

namespace waka::common {

static string logLevelToString(int level) {
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
  assert(false);
}

static int stringToLogLevel(const string& str) {
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

void Config::store() const {
  ConfigMapper mapper;
  mapper.update({"version", version_});
  mapper.update({"ip", ip_});
  mapper.update({"time_format", time_format_});
  mapper.update({"log_level", logLevelToString(log_level_)});
  mapper.update({"timeout", to_string(timeout_)});
  mapper.update({"port", to_string(port_)});
}

void Config::load() {
  ConfigMapper mapper;
  auto config = mapper.get("version");
  version_ = config.value();
  config = mapper.get("ip");
  ip_ = config.value();
  config = mapper.get("time_format");
  time_format_ = config.value();
  config = mapper.get("log_level");
  log_level_ = stringToLogLevel(config.value());
  config = mapper.get("timeout");
  timeout_ = atoi(config.value().c_str());
  config = mapper.get("port");
  timeout_ = atoi(config.value().c_str());
}

}  // namespace waka::common
