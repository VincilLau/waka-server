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

#include "date.hpp"

#include <cassert>
#include <exception/date_error.hpp>
#include <regex>

using fmt::format;
using std::int64_t;
using std::memset;
using std::regex;
using std::regex_match;
using std::smatch;
using std::string;

using waka::exception::DateError;

namespace waka::common {

bool Date::operator<(const Date& other) const {
  if (year_ < other.year_) {
    return true;
  } else if (year_ > other.year_) {
    return false;
  }

  if (month_ < other.month_) {
    return true;
  } else if (month_ > other.month_) {
    return false;
  }

  return day_ < other.day_;
}

bool Date::operator>(const Date& other) const {
  if (year_ > other.year_) {
    return true;
  } else if (year_ < other.year_) {
    return false;
  }

  if (month_ > other.month_) {
    return true;
  } else if (month_ < other.month_) {
    return false;
  }

  return day_ > other.day_;
}

bool Date::operator<=(const Date& other) const {
  if (year_ < other.year_) {
    return true;
  } else if (year_ > other.year_) {
    return false;
  }

  if (month_ < other.month_) {
    return true;
  } else if (month_ > other.month_) {
    return false;
  }

  return day_ <= other.day_;
}

bool Date::operator>=(const Date& other) const {
  if (year_ > other.year_) {
    return true;
  } else if (year_ < other.year_) {
    return false;
  }

  if (month_ > other.month_) {
    return true;
  } else if (month_ < other.month_) {
    return false;
  }

  return day_ >= other.day_;
}

bool Date::leapYear() const {
  if (year_ % 4 != 0) {
    return false;
  }
  if (year_ % 400 == 0) {
    return true;
  }
  if (year_ % 100 == 0) {
    return false;
  }
  return true;
}

int Date::getMonthDay() const {
  assert(year_ >= 2000 && year_ <= 2099);
  assert(month_ >= 1 && month_ <= 12);

  int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  month_days[1] += leapYear();
  return month_days[month_ - 1];
}

bool Date::valid() const {
  if (year_ < 2000 || year_ > 2099) {
    return false;
  }
  if (month_ < 1 || month_ > 12) {
    return false;
  }
  int month_day = getMonthDay();
  if (day_ < 1 || day_ > month_day) {
    return false;
  }
  return true;
}

void Date::validate() {
  assert(month_ > 0 && month_ <= 12);
  // 因为只有++运算符，所以day_不可能大于32
  assert(day_ > 0 && day_ <= 32);

  int month_day = getMonthDay();
  if (day_ <= month_day) {
    if (!valid()) {
      throw DateError(format("invalid date {}", toString()));
    }

    // 如果day_没有溢出，则month_和year_也没有溢出
    return;
  }

  assert(day_ == month_day + 1);
  month_++;
  year_ += month_ / 12;
  month_ %= 12;
  day_ = 1;

  if (!valid()) {
    throw DateError(format("invalid date after validated {}", toString()));
  }
}

int64_t Date::unixMilli() const {
  assert(valid());

  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  tm.tm_year = year_ - 1900;
  tm.tm_mon = month_ - 1;
  tm.tm_mday = day_;
  time_t t = mktime(&tm);
  assert(t != -1);
  return static_cast<int64_t>(t) * 1000;
}

Date Date::fromUnixMilli(int64_t msec) {
  // 先限制一下
  assert(msec < 25 * 365 * 24 * 3600 * 1000);
  assert(msec < 135 * 365 * 24 * 3600 * 1000);

  time_t t = msec / 1000;
  struct tm* tm = localtime(&t);
  int year = tm->tm_year + 1900;
  int month = tm->tm_mon + 1;
  int day = tm->tm_mday;
  return {year, month, day};
}

Date Date::today() {
  time_t t = time(nullptr);
  struct tm* tm = localtime(&t);
  int year = tm->tm_year + 1900;
  int month = tm->tm_mon + 1;
  int day = tm->tm_mday;
  return {year, month, day};
}

Date Date::parse(const string& date_str) {
  regex pattern{"^(\\d{4})-(\\d{2})-(\\d{2})$"};
  smatch matches;
  if (!regex_match(date_str, matches, pattern)) {
    throw DateError(format("parse date '{}' failed", date_str));
  }
  assert(matches.size() == 4);

  int year = atoi(matches[1].str().c_str());
  int month = atoi(matches[2].str().c_str());
  int day = atoi(matches[3].str().c_str());
  if (year == 0 || month == 0 || day == 0) {
    throw DateError(format("parse date '{}' failed", date_str));
  }

  Date date{year, month, day};
  if (!date.valid()) {
    throw DateError(format("parse date '{}' failed", date_str));
  }
  return date;
}

}  // namespace waka::common
