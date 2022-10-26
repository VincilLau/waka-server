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

#include "meta.hpp"

#include <common/date.hpp>
#include <common/log.hpp>
#include <common/pattern.hpp>
#include <define.hpp>
#include <exception/date_error.hpp>
#include <exception/meta_data_error.hpp>

using std::string;
using std::to_string;
using waka::common::Date;
using waka::common::isValidIP;
using waka::common::MetaData;
using waka::common::strToLogLevel;
using waka::exception::DateError;
using waka::exception::MetaDataError;

namespace waka::service {

void MetaService::init() const {
  mapper_.createTable();
  MetaData meta_data;
  mapper_.insert({"create_date", meta_data.create_date.toString()});
  mapper_.insert({"ip", meta_data.ip});
  mapper_.insert({"log_level", meta_data.log_level});
  mapper_.insert({"port", to_string(meta_data.port)});
  mapper_.insert({"time_format", meta_data.time_format});
  mapper_.insert({"timeout", to_string(meta_data.timeout)});
  mapper_.insert({"version", WAKA_VERSION});
}

MetaData MetaService::readMetaData() const {
  MetaData meta_data;
  string create_date_value = mapper_.get("create_date");
  try {
    meta_data.create_date = Date::parse(create_date_value);
  } catch (const DateError& e) {
    throw MetaDataError("create_date", create_date_value);
  }

  meta_data.ip = mapper_.get("ip");
  if (!isValidIP(meta_data.ip)) {
    throw MetaDataError("ip", meta_data.ip);
  }

  meta_data.log_level = mapper_.get("log_level");
  int level = strToLogLevel(meta_data.log_level);
  if (level == -1) {
    throw MetaDataError("log_level", meta_data.log_level);
  }

  string port_value = mapper_.get("port");
  int port = atoi(port_value.c_str());
  if (port < 1 || port > UINT16_MAX) {
    throw MetaDataError("port", port_value);
  }
  meta_data.port = port;

  meta_data.time_format = mapper_.get("time_format");

  string timeout_value = mapper_.get("timeout");
  int timeout = atoi(timeout_value.c_str());
  if (timeout < MetaData::kMinTimeout || timeout > MetaData::kMaxTimeout) {
    throw MetaDataError("timeout", timeout_value);
  }
  meta_data.timeout = timeout;

  meta_data.version = mapper_.get("version");

  return meta_data;
}

void MetaService::writeMetaData(const MetaData& meta_data) const {
  if (!isValidIP(meta_data.ip)) {
    throw MetaDataError("ip", meta_data.ip);
  }
  if (strToLogLevel(meta_data.log_level) == -1) {
    throw MetaDataError("log_level", meta_data.log_level);
  }
  if (meta_data.port == 0) {
    throw MetaDataError("port", "0");
  }
  if (meta_data.timeout < MetaData::kMinTimeout ||
      meta_data.timeout > MetaData::kMaxTimeout) {
    throw MetaDataError("timeout", to_string(meta_data.timeout));
  }

  mapper_.update({"create_date", meta_data.create_date.toString()});
  mapper_.update({"ip", meta_data.ip});
  mapper_.update({"log_level", meta_data.log_level});
  mapper_.update({"port", to_string(meta_data.port)});
  mapper_.update({"time_format", meta_data.time_format});
  mapper_.update({"timeout", to_string(meta_data.timeout)});
  mapper_.update({"version", meta_data.version});
}

}  // namespace waka::service
