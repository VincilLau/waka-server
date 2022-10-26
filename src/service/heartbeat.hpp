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

#ifndef WAKA_SRC_SERVICE_HEARTBEAT_SERVICE_HPP_
#define WAKA_SRC_SERVICE_HEARTBEAT_SERVICE_HPP_

#include <bo/heartbeat.hpp>
#include <bo/summary.hpp>
#include <common/date.hpp>
#include <dao/heartbeat.hpp>

namespace waka::service {

class HeartbeatService {
 public:
  // 保存一个心跳
  // 返回heatbeat的UUID
  [[nodiscard]] std::string save(bo::Heartbeat heartbeat) const;
  // 获取今日时间的毫秒数
  [[nodiscard]] std::int64_t today() const;
  // 获取指定时间区间(闭区间)内的汇总数据
  [[nodiscard]] bo::Summary summarize(const common::Date& start,
                                      const common::Date& end);

 private:
  dao::HeartbeatMapper mapper_;
};

}  // namespace waka::service

#endif  // WAKA_SRC_SERVICE_HEARTBEAT_SERVICE_HPP_
