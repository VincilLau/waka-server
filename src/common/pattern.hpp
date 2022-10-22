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

#ifndef WAKA_SRC_COMMON_RE_HPP_
#define WAKA_SRC_COMMON_RE_HPP_

#include <cassert>
#include <regex>
#include <string>

namespace waka::common {

// 判断一个字符串是否是合法的IP
[[nodiscard]] inline static bool isValidIP(const std::string& ip) {
  std::regex pattern{
      "^"
      "((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})"
      "(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2}))"
      "{3}"
      "$"};
  return std::regex_match(ip, pattern);
}

// 返回值为{os, editor}
[[nodiscard]] inline static std::pair<std::string, std::string> parseUserAgent(
    const std::string& ua) {
  std::regex pattern("^wakatime/.+ \\((.+?)-.+-.+\\) .+ (.+)/.+$");
  std::smatch matches;
  if (std::regex_match(ua, matches, pattern)) {
    assert(matches.size() == 2);
    return {matches[0], matches[1]};
  }
  return {"unknown", "unknown"};
}

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_RE_HPP_
