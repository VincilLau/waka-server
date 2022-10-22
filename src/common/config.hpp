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

#ifndef WAKA_SRC_COMMON_CONFIG_HPP_
#define WAKA_SRC_COMMON_CONFIG_HPP_

#include <cstdint>
#include <memory>
#include <string>

namespace waka::common {

class Config {
 public:
  static constexpr int kMaxTimeout = 300;

  [[nodiscard]] const std::string& ip() const { return ip_; }
  [[nodiscard]] const std::string& timeFormat() const { return time_format_; }
  [[nodiscard]] const std::string& logLevel() const { return log_level_; }
  [[nodiscard]] int timeout() const { return timeout_; }
  [[nodiscard]] std::uint16_t port() const { return port_; }

  void setIP(std::string ip) { ip_ = std::move(ip); }
  void setTimeFormat(std::string time_format) { time_format_ = time_format; }
  void setLogLevel(std::string level) { log_level_ = std::move(level); }
  void setTimeout(int timeout) { timeout_ = timeout; }
  void setPort(std::uint16_t port) { port_ = port; }

  static const Config& getConfig();
  static void setConfig(Config config);

 private:
  static std::unique_ptr<Config> config_;

  std::string ip_ = "127.0.0.1";            // 服务器绑定的IP地址
  std::string time_format_ = "%H小时%M分";  // 时间的可视化格式
  std::string log_level_ = "info";          // spdlog的日志等级
  int timeout_ = 135;                       // 键盘输入的超时时间
  std::uint16_t port_ = 8080;               // 服务器绑定的端口号
};

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_CONFIG_HPP_
