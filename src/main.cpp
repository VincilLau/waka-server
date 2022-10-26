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

#include <CLI/CLI.hpp>
#include <common/args.hpp>
#include <common/log.hpp>
#include <service/meta.hpp>

using fmt::format;
using std::string;
using waka::common::Args;
using waka::common::MetaData;
using waka::common::strToLogLevel;

static string logLevelValidator(string& str) {
  int level = strToLogLevel(str);
  if (level == -1) {
    return "无效的日志级别";
  }
  return {};
}

int main(int argc, char** argv) {
  CLI::App app{"waka-server"};
  Args args;

  app.add_flag("--info", args.info, "显示配置信息");
  app.add_flag("-i, --install", args.install, "安装waka-server");
  app.add_option("--set-ip", args.ip, "设置服务器绑定的IP")
      ->check(CLI::ValidIPV4);
  app.add_option("--set-log-level", args.log_level,
                 "设置日志级别，可以为trace,debug,info,warn,error,critical,off")
      ->check(CLI::Validator(logLevelValidator, ""));
  app.add_option("--set-port", args.port, "设置服务器监听的端口")
      ->check(CLI::Range(1, UINT16_MAX));
  app.add_option("--set-time-format", args.time_format, "设置时间显示格式");
  app.add_option("--set-timeout", args.timeout, "设置键盘超时时间")
      ->check(CLI::Range(MetaData::kMinTimeout, MetaData::kMaxTimeout));

  app.set_version_flag("-v, --version", format("version {}", WAKA_VERSION),
                       "显示版本信息");
  app.set_help_flag("-h, --help", "显示帮助信息");
  CLI11_PARSE(app, argc, argv);

  return args.exec();
}
