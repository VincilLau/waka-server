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

#ifndef WAKA_SRC_EXCEPTION_EXCEPTION_HPP_
#define WAKA_SRC_EXCEPTION_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace waka::exception {

// waka-server中所有异常的基类
class Exception : public std::exception {
 public:
  explicit Exception(std::string reason) : reason_(std::move(reason)) {}

  [[nodiscard]] const char* what() const noexcept override {
    return reason_.c_str();
  }

 private:
  std::string reason_;  // 触发异常的原因
};

}  // namespace waka::exception

#endif  // WAKA_SRC_EXCEPTION_EXCEPTION_HPP_
