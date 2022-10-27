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

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <common/date.hpp>
#include <exception/date_error.hpp>

using waka::exception::DateError;

namespace waka::common {

TEST(DateTest, testIncrease) {
  Date date{2022, 10, 26};
  date++;
  EXPECT_EQ(date.year(), 2022);
  EXPECT_EQ(date.month(), 10);
  EXPECT_EQ(date.day(), 27);

  date = {2022, 12, 31};
  date++;
  EXPECT_EQ(date.year(), 2023);
  EXPECT_EQ(date.month(), 1);
  EXPECT_EQ(date.day(), 1);

  date = {2019, 2, 28};
  date++;
  EXPECT_EQ(date.year(), 2019);
  EXPECT_EQ(date.month(), 3);
  EXPECT_EQ(date.day(), 1);

  date = {2020, 2, 28};
  date++;
  EXPECT_EQ(date.year(), 2020);
  EXPECT_EQ(date.month(), 2);
  EXPECT_EQ(date.day(), 29);

  date = {2099, 12, 31};
  EXPECT_THROW(date++, DateError);
}

TEST(DateTest, testEqual) {
  Date date1{2022, 10, 26};
  Date date2{2022, 10, 26};
  EXPECT_EQ(date1, date2);

  Date date3{2022, 10, 26};
  Date date4{2022, 10, 27};
  EXPECT_NE(date3, date4);
}

TEST(DateTest, testCompare) {
  Date date1{2022, 10, 26};
  Date date2{2022, 10, 27};
  EXPECT_LT(date1, date2);
  EXPECT_LE(date1, date2);
  EXPECT_GT(date2, date1);
  EXPECT_GE(date2, date1);

  date1 = {2020, 2, 28};
  date2 = {2020, 2, 29};
  EXPECT_LT(date1, date2);
  EXPECT_LE(date1, date2);
  EXPECT_GT(date2, date1);
  EXPECT_GE(date2, date1);

  date1 = {2022, 2, 28};
  date2 = {2022, 3, 1};
  EXPECT_LT(date1, date2);
  EXPECT_LE(date1, date2);
  EXPECT_GT(date2, date1);
  EXPECT_GE(date2, date1);

  date1 = {2022, 10, 26};
  date2 = {2022, 10, 26};
  EXPECT_LE(date1, date2);
  EXPECT_GE(date2, date1);
}

TEST(DateTest, testLeapYear) {
  Date date{2020, 1, 1};
  EXPECT_TRUE(date.leapYear());

  date = {2022, 1, 1};
  EXPECT_FALSE(date.leapYear());
}

TEST(DateTest, testGetMonthDay) {
  Date date{2022, 1, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2022, 2, 1};
  EXPECT_EQ(date.getMonthDay(), 28);
  date = {2022, 3, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2022, 4, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2022, 5, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2022, 6, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2022, 7, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2022, 8, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2022, 9, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2022, 10, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2022, 11, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2022, 12, 1};
  EXPECT_EQ(date.getMonthDay(), 31);

  date = {2020, 1, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2020, 2, 1};
  EXPECT_EQ(date.getMonthDay(), 29);
  date = {2020, 3, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2020, 4, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2020, 5, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2020, 6, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2020, 7, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2020, 8, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2020, 9, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2020, 10, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
  date = {2020, 11, 1};
  EXPECT_EQ(date.getMonthDay(), 30);
  date = {2020, 12, 1};
  EXPECT_EQ(date.getMonthDay(), 31);
}

TEST(DateTest, testToString) {
  Date date{2022, 10, 26};
  EXPECT_EQ(date.toString(), "2022-10-26");
  date = {2022, 1, 8};
  EXPECT_EQ(date.toString(), "2022-01-08");
}

TEST(DateTest, testParse) {
  Date date = Date::parse("2022-10-26");
  Date want{2022, 10, 26};
  EXPECT_EQ(date, want);

  date = Date::parse("2022-02-28");
  want = {2022, 2, 28};
  EXPECT_EQ(date, want);

  date = Date::parse("2020-02-29");
  want = {2020, 2, 29};
  EXPECT_EQ(date, want);

  EXPECT_THROW(date = Date::parse("2022-02-29"), DateError);
  EXPECT_THROW(date = Date::parse("2022-11-31"), DateError);
  EXPECT_THROW(date = Date::parse("1999-01-01"), DateError);
  EXPECT_THROW(date = Date::parse("2022-1-1"), DateError);
  EXPECT_THROW(date = Date::parse("2100-12-08"), DateError);
  EXPECT_THROW(date = Date::parse("0000-12-08"), DateError);
  EXPECT_THROW(date = Date::parse("2022-02-2*"), DateError);
  EXPECT_THROW(date = Date::parse("2022*02*28"), DateError);
  EXPECT_THROW(date = Date::parse("12345-12-08"), DateError);
  EXPECT_THROW(date = Date::parse("hello world"), DateError);
}

TEST(DateTest, testUnixMilli) {
  Date date{2022, 10, 27};
  EXPECT_EQ(date.unixMilli(), 1666800000000LL);
}

TEST(DateTest, testFromUnixMilli) {
  Date date = Date::fromUnixMilli(1666800000000LL);
  Date want{2022, 10, 27};
  EXPECT_EQ(date, want);
}

TEST(Date, testValid) {
  Date date{2022, 10, 27};
  EXPECT_TRUE(date.valid());
}

}  // namespace waka::common

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  spdlog::set_level(spdlog::level::trace);
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%l] %t [%@] -- %v");
  return RUN_ALL_TESTS();
}
