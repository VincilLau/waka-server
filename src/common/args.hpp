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

#ifndef WAKA_SRC_COMMON_ARGS_HPP_
#define WAKA_SRC_COMMON_ARGS_HPP_

#include <string>

namespace waka::common {

struct Args {
  // 根据命令行参数执行响应操作
  [[nodiscard]] int exec() const;

  bool info = false;
  std::string ip;
  bool install = false;
  std::string log_level;
  int port = 0;
  std::string time_format;
  int timeout = 0;
};

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_ARGS_HPP_
