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

#include <common/log.hpp>
#include <common/re.hpp>
#include <dao/config_mapper.hpp>

using std::string;
using std::to_string;
using waka::dao::ConfigMapper;

namespace waka::common {

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
  port_ = atoi(config.value().c_str());
}

}  // namespace waka::common
