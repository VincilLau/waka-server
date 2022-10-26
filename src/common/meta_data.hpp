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

#ifndef WAKA_SRC_COMMON_META_DATA_HPP_
#define WAKA_SRC_COMMON_META_DATA_HPP_

#include <cassert>
#include <cstdint>
#include <define.hpp>
#include <memory>
#include <string>

#include "date.hpp"

namespace waka::common {

// waka-server的元数据
// 在服务器启动时从数据库的meta表加载
struct MetaData {
 public:
  // 键盘超时的最小时间，单位分钟
  static constexpr int kMinTimeout = 3;
  // 键盘超时的最大时间，单位分钟
  static constexpr int kMaxTimeout = 60;
  // 键盘超时的默认时间，单位分钟
  static constexpr int kDefaultTimeout = 3;

  // 获取全局元数据实例
  static std::shared_ptr<MetaData> getInstance() {
    assert(instance_ != nullptr);
    return instance_;
  }
  // 设置全局元数据实例
  static void setInstance(std::shared_ptr<MetaData> meta_data) {
    assert(instance_ == nullptr);
    instance_ = meta_data;
  }

 private:
  // 设置元数据实例
  static std::shared_ptr<MetaData> instance_;

 public:
  Date create_date = Date::today();        // 数据库的创建日期
  std::string ip = "127.0.0.1";            // 服务器绑定的IP地址
  std::string log_level = "info";          // spdlog的日志等级
  std::uint16_t port = 8080;               // 服务器绑定的端口号
  std::string time_format = "%H小时%M分";  // 时间的可视化格式
  int timeout = kDefaultTimeout;           // 键盘输入的超时时间
  std::string version = WAKA_VERSION;      // 服务器版本
};

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_META_DATA_HPP_
