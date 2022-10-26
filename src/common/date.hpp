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

#ifndef WAKA_SRC_COMMON_DATE_HPP_
#define WAKA_SRC_COMMON_DATE_HPP_

#include <fmt/core.h>

namespace waka::common {

// 表示日期的类
// 年份的范围2000-2099，闭区间
// 计算时间戳时不考虑闰秒
class Date {
 public:
  // 允许构造不合法的Date，使用时需要调用valid()检查
  Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

  [[nodiscard]] int year() const { return year_; }
  [[nodiscard]] int month() const { return month_; }
  [[nodiscard]] int day() const { return day_; }

  // 日期加一天
  Date& operator++() {
    day_++;
    validate();
    return *this;
  }
  Date& operator++(int) {
    day_++;
    validate();
    return *this;
  }

  [[nodiscard]] bool operator==(const Date& other) const {
    return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
  }
  [[nodiscard]] bool operator!=(const Date& other) const {
    return year_ != other.year_ || month_ != other.month_ || day_ != other.day_;
  }

  [[nodiscard]] bool operator<(const Date& other) const;
  [[nodiscard]] bool operator>(const Date& other) const;
  [[nodiscard]] bool operator<=(const Date& other) const;
  [[nodiscard]] bool operator>=(const Date& other) const;

  // 平年返回false，闰年返回true
  [[nodiscard]] bool leapYear() const;
  // 获取当月的天数
  [[nodiscard]] int getMonthDay() const;

  // 获取日期的字符串形式，格式为YYYY-MM-DD
  // 因为在报错时要用到该函数，所以该函数不对日期的有效性作要求
  [[nodiscard]] std::string toString() const {
    return fmt::format("{:04d}-{:02d}-{:02d}", year_, month_, day_);
  }
  // 解析YYYY-MM-DD格式的日期
  // 如果格式错误或日期不合法，将抛出DateError异常
  [[nodiscard]] static Date parse(const std::string& date_str);

  // 获取当前日期零时的时间戳，单位毫秒
  [[nodiscard]] std::int64_t unixMilli() const;
  // 获取指定时间戳的日期，单位毫秒
  // 时间可以不为某一天的零时
  [[nodiscard]] static Date fromUnixMilli(std::int64_t msec);

  // 获取今天的日期
  [[nodiscard]] static Date today();

 private:
  // 判断当前日期是否符合以下条件
  // 1. 年份的范围为2000-2099，闭区间
  // 2. 月份范围为1-12
  // 3. day_的值在1到当月天数这一区间内
  [[nodiscard]] bool valid() const;

  // 处理对日期加减产生的进位，使日期合法
  // 当前Date类只支持++运算符，所以只处理进位，不考虑借位
  // 如果年份超出范围(2000-2099)，抛出DateError异常
  void validate();

  int year_;
  int month_;
  int day_;
};

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_DATE_HPP_
