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

#ifndef WAKA_SRC_BO_SUMMARIES_HPP_
#define WAKA_SRC_BO_SUMMARIES_HPP_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace waka::bo {

struct Summaries {
  std::unordered_map<std::string, std::int64_t> categories;
  std::unordered_map<std::string, std::int64_t> editors;
  std::unordered_map<std::string, std::int64_t> languages;
  std::unordered_map<std::string, std::int64_t> oss;
  std::unordered_map<std::string, std::int64_t> projects;
  std::int64_t total_msec = 0;
};

}  // namespace waka::bo

#endif  // WAKA_SRC_BO_SUMMARIES_HPP_
