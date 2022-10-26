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

#ifndef WAKA_SRC_COMMON_PATTERN_HPP_
#define WAKA_SRC_COMMON_PATTERN_HPP_

#include "meta_data.hpp"

namespace waka::common {

// 判断一个字符串是否是合法的IP
[[nodiscard]] bool isValidIP(const std::string& ip);
// 解析user_agent，如果editor或os未知，返回'Unknown'
void parseUserAgent(const std::string& ua, std::string& os,
                    std::string& editor);
// 根据元数据中的time_format项格式化时长，时长的单位为毫秒
[[nodiscard]] std::string formatTime(std::int64_t msec);

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_PATTERN_HPP_
