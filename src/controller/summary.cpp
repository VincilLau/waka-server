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

#include "summary.hpp"

#include <spdlog/spdlog.h>

#include <common/pattern.hpp>
#include <dto/summary/get.hpp>
#include <exception/date_error.hpp>
#include <http/msg.hpp>

using fmt::format;
using std::int64_t;
using std::string;
using waka::bo::Summary;
using waka::common::Date;
using waka::common::formatTime;
using waka::dto::summary::get::Item;
using waka::dto::summary::get::Result;
using waka::exception::DateError;
using waka::http::jsonMsg;
using waka::http::Request;
using waka::http::Response;
using waka::http::Status;

namespace waka::controller {

bool SummaryController::parseGetParam(const Request& req, Response& resp,
                                      Date& start, Date& end) {
  string start_param = req.getParam("start");
  string end_param = req.getParam("end");

  try {
    start = Date::parse(start_param);
    end = Date::parse(end_param);
  } catch (const DateError& e) {
    resp.setStatus(Status::kBadRequest);
    resp.setContent(jsonMsg(e.what()), "application/json");
    return false;
  }

  if (start > end) {
    SPDLOG_WARN(
        format("GET /api/summary 400, msg='start({}) can't be greater "
               "than end({})'",
               start.toString(), end.toString()));
    resp.setStatus(Status::kBadRequest);
    resp.setContent(jsonMsg("start can't be greater than end"),
                    "application/json");

    return false;
  }

  return true;
}

static Result summaryToResult(Summary summary, const Date& start,

                              const Date& end) {
  SPDLOG_DEBUG("GET /api/summary");

  Result result;
  result.total.start = start.toString();
  result.total.end = end.toString();
  result.total.time_text = formatTime(summary.total_msec);
  result.total.total_msec = summary.total_msec;
  for (int64_t msec : summary.daily_msec) {
    result.days.push_back({msec, formatTime(msec)});
  }

  for (auto& i : summary.editors) {
    Item s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.editors.array.push_back(std::move(s));
  }

  for (auto& i : summary.languages) {
    Item s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.languages.array.push_back(std::move(s));
  }

  for (auto& i : summary.oss) {
    Item s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.oss.array.push_back(std::move(s));
  }

  for (auto& i : summary.projects) {
    Item s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.projects.array.push_back(std::move(s));
  }

  return result;
}

void SummaryController::get(const Request& req, Response& resp) {
  Date start = Date::today();
  Date end = start;
  bool ok = parseGetParam(req, resp, start, end);
  if (!ok) {
    return;
  }

  Summary summary = heartbeat_service_.summarize(start, end);
  Result result = summaryToResult(std::move(summary), start, end);
  resp.setStatus(Status::kOK);
  resp.setContent(result.toJSON().dump(), "application/json");
  SPDLOG_INFO("GET /api/summary 200");
}

}  // namespace waka::controller
