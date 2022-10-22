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

#include "meta_service.hpp"

#include <common/log.hpp>
#include <common/pattern.hpp>
#include <exception/config_error.hpp>
#include <version.hpp>

using std::string;
using std::to_string;
using waka::common::Config;
using waka::common::isValidIP;
using waka::common::stringToLogLevel;
using waka::exception::ConfigError;

namespace waka::service {

void MetaService::init() const {
  mapper_.createTable();
  mapper_.insert({"version", kVersion});
  mapper_.insert({"ip", "127.0.0.1"});
  mapper_.insert({"port", "8080"});
  mapper_.insert({"log_level", "info"});
  mapper_.insert({"time_format", "%H小时%M分"});
  mapper_.insert({"timeout", "135"});
}

Config MetaService::loadConfig() const {
  Config config;
  config.setIP(mapper_.get("ip"));
  if (!isValidIP(config.ip())) {
    throw ConfigError("ip", config.ip());
  }

  string portStr = mapper_.get("port");
  int port = atoi(portStr.c_str());
  if (port < 1 || port > 65535) {
    throw ConfigError("port", portStr);
  }
  config.setPort(port);

  string logLevel = mapper_.get("log_level");
  int level = stringToLogLevel(logLevel);
  if (level == -1) {
    throw ConfigError("log_level", logLevel);
  }
  config.setLogLevel(std::move(logLevel));

  config.setTimeFormat(mapper_.get("time_format"));

  string timeoutStr = mapper_.get("timeout");
  int timeout = atoi(timeoutStr.c_str());
  if (timeout < 1 || timeout > Config::kMaxTimeout) {
    throw ConfigError("timeout", timeoutStr);
  }
  config.setTimeout(timeout);

  return config;
}

void MetaService::storeConfig(const Config& config) const {
  if (!isValidIP(config.ip())) {
    throw ConfigError("ip", config.ip());
  }
  if (stringToLogLevel(config.logLevel()) == -1) {
    throw ConfigError("log_level", config.logLevel());
  }
  if (config.port() == 0) {
    throw ConfigError("port", "0");
  }
  if (config.timeout() < 1 || config.timeout() > Config::kMaxTimeout) {
    throw ConfigError("timeout", to_string(config.timeout()));
  }

  mapper_.update({"ip", config.ip()});
  mapper_.update({"port", to_string(config.port())});
  mapper_.update({"log_level", config.logLevel()});
  mapper_.update({"time_format", config.timeFormat()});
  mapper_.update({"timeout", to_string(config.timeout())});
}

}  // namespace waka::service
