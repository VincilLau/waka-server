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

#ifndef WAKA_SRC_COMMON_LOG_HPP_
#define WAKA_SRC_COMMON_LOG_HPP_

#include <string>

namespace waka::common {

// 初始化日志记录器，将进行以下操作
// 1. 将日志输出到${date_dir}/log目录
// 2. 日志按日期分割
// 3. 暂时设置日志等级为trace
// 4. 暂时设置每条日志都刷盘
// 5. 修改日志格式
// 6. 如果在Linux系统上且stdout为终端，将日志输出到stdout
void initLogger(const std::string& data_dir);

// 将日志级别从整数形式转换为字符串形式
// 例如从2转换为info
[[nodiscard]] const char* logLevelToStr(int level);
// 将日志级别从字符串形式转换为整数形式
// 例如从info转换为2
[[nodiscard]] int strToLogLevel(const std::string& str);

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_LOG_HPP_
