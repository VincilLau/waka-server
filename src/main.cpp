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

#include <common/log.hpp>
#include <dao/heartbeat.hpp>
#include <http/routing.hpp>
#include <service/meta.hpp>

using httplib::Server;
using std::make_shared;
using waka::common::initLogger;
using waka::common::MetaData;
using waka::common::strToLogLevel;
using waka::dao::DB;
using waka::dao::HeartbeatMapper;
using waka::http::setupRouting;
using waka::service::MetaService;

static void init() {
  initLogger(WAKA_DATA_DIR);

  bool exists = DB::exists(WAKA_DATA_DIR);
  auto db = make_shared<DB>();
  db->open(WAKA_DATA_DIR);
  DB::setInstance(db);

  MetaService meta_service;
  if (!exists) {
    meta_service.init();
  }
  auto meta_data = make_shared<MetaData>(meta_service.readMetaData());
  MetaData::setInstance(meta_data);

  int level = strToLogLevel(meta_data->log_level);
  spdlog::set_level(static_cast<spdlog::level::level_enum>(level));

  HeartbeatMapper{}.loadTableSet();
}

static void runServer() {
  Server server;
  setupRouting(server);
  auto meta_data = MetaData::getInstance();
  SPDLOG_INFO("listen on {}:{}", meta_data->ip, meta_data->port);
  server.listen(meta_data->ip.c_str(), meta_data->port);
}

int main() {
  try {
    init();
    runServer();
  } catch (const std::exception& e) {
    SPDLOG_CRITICAL("{}", e.what());
    return 1;
  }
  return 0;
}
