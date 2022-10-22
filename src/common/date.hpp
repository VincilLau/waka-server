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

#include <fmt/core.h>

#include <cstdint>
#include <ctime>
#include <string>

#ifndef WAKA_SRC_COMMON_DATE_HPP_
#define WAKA_SRC_COMMON_DATE_HPP_

namespace waka::common {

class Date {
 public:
  Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

  [[nodiscard]] int year() const { return year_; }
  [[nodiscard]] int month() const { return month_; }
  [[nodiscard]] int day() const { return day_; }

  [[nodiscard]] std::string toString() const {
    return fmt::format("{:04d}-{:02d}-{:02d}", year_, month_, day_);
  }

  [[nodiscard]] static Date fromUnixMilli(std::int64_t msec) {
    time_t t = msec / 1000;
    struct tm* tm = localtime(&t);
    return {tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday};
  }

 private:
  int year_;
  int month_;
  int day_;
};

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_DATE_HPP_
