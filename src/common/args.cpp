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

#include "args.hpp"

#include <spdlog/spdlog.h>

#include <dao/db.hpp>
#include <dao/heartbeat.hpp>
#include <define.hpp>
#include <http/routing.hpp>
#include <iostream>
#include <service/meta.hpp>

#include "log.hpp"
#include "meta_data.hpp"

using fmt::print;
using httplib::Server;
using std::cerr;
using std::cout;
using std::endl;
using std::make_shared;
using waka::dao::DB;
using waka::dao::HeartbeatMapper;
using waka::http::setupRouting;
using waka::service::MetaService;

namespace waka::common {

static void init() {
  initLogger(WAKA_DATA_DIR);

  auto db = make_shared<DB>();
  db->open(WAKA_DATA_DIR);
  DB::setInstance(db);

  auto meta_data = make_shared<MetaData>(MetaService{}.readMetaData());
  MetaData::setInstance(meta_data);

  auto level = static_cast<spdlog::level::level_enum>(
      strToLogLevel(meta_data->log_level));
  spdlog::set_level(level);
  spdlog::flush_on(level);

  HeartbeatMapper{}.loadTableSet();
}

static void installWakaServer() {
  initLogger(WAKA_DATA_DIR);
  auto db = make_shared<DB>();
  db->open(WAKA_DATA_DIR);
  DB::setInstance(db);
  MetaService{}.init();
  db->close();
  cout << "安装完成！" << endl;
}

static void runServer() {
  Server server;
  setupRouting(server);
  auto meta_data = MetaData::getInstance();
  SPDLOG_INFO("listen on {}:{}", meta_data->ip, meta_data->port);
  server.listen(meta_data->ip.c_str(), meta_data->port);
}

static void printInfo() {
  auto meta_data = MetaData::getInstance();
  print("服务器版本: {}\n", meta_data->version);
  print("安装时间: {}\n", meta_data->create_date.toString());
  print("IP地址: {}\n", meta_data->ip);
  print("端口号: {}\n", meta_data->port);
  print("日志级别: {}\n", meta_data->log_level);
  print("时间格式: {}\n", meta_data->time_format);
  print("键盘超时时间: {}分钟\n", meta_data->timeout);
}

int Args::exec() const {
  bool exists = DB::exists(WAKA_DATA_DIR);

  if (install) {
    if (exists) {
      cerr << "检测到数据库，您可能已经完成安装！" << endl;
      return 1;
    }
    try {
      installWakaServer();
      return 0;
    } catch (const std::exception& e) {
      return 1;
    }
  }

  if (!exists) {
    cerr << "没有检测到数据库，您可能尚未进行安装！" << endl;
    return 1;
  }

  init();
  MetaData meta_data = *MetaData::getInstance();
  bool run_server = true;

  if (info) {
    run_server = false;
    printInfo();
  }
  if (!ip.empty()) {
    run_server = false;
    meta_data.ip = std::move(ip);
  }
  if (!log_level.empty()) {
    run_server = false;
    meta_data.log_level = std::move(log_level);
  }
  if (port > 0) {
    run_server = false;
    meta_data.port = port;
  }
  if (!time_format.empty()) {
    run_server = false;
    meta_data.time_format = std::move(time_format);
  }
  if (timeout > 0) {
    run_server = false;
    meta_data.timeout = timeout;
  }

  if (!run_server) {
    MetaService{}.writeMetaData(meta_data);
  } else {
    runServer();
  }

  return 0;
}

}  // namespace waka::common
