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

#include <httplib.h>
#include <spdlog/spdlog.h>

#include <common/config.hpp>
#include <common/log.hpp>
#include <controller/routing.hpp>
#include <dao/db.hpp>
#include <stdexcept>

using httplib::Server;
using std::exception;
using waka::common::applyLogLevel;
using waka::common::Config;
using waka::common::initLogger;
using waka::controller::setupRouting;
using waka::dao::initDB;

static void init() {
  initLogger();
  // 打开数据库
  // 如果数据库不存在则创建数据库并初始化meta表
  // 加载数据库中的heartbeat表
  initDB();
  // 从meta表中读取配置
  Config::init();
  // 根据配置设置日志级别
  applyLogLevel();
}

static void runServer() {
  Server server;
  setupRouting(server);
  const Config& config = Config::get();
  SPDLOG_INFO("listen on {}:{}", config.ip(), config.port());
  server.listen(config.ip().c_str(), config.port());
}

int main() {
  try {
    init();
    runServer();
  } catch (const exception& e) {
    SPDLOG_CRITICAL("{}", e.what());
    return 1;
  }
  return 0;
}
