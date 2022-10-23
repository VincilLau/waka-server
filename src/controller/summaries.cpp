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

#include "summaries.hpp"

#include <spdlog/spdlog.h>

#include <common/date.hpp>
#include <common/http.hpp>
#include <common/pattern.hpp>
#include <dto/summary/get.hpp>
#include <service/heartbeat_service.hpp>

#include "msg.hpp"

using fmt::format;
using httplib::Request;
using httplib::Response;
using std::string;
using waka::common::Date;
using waka::common::formatTime;
using waka::common::HttpStatus;
using waka::dto::summary::get::Result;
using waka::dto::summary::get::Summary;
using waka::exception::DateError;
using waka::service::HeartbeatService;

namespace waka::controller {

void getSummaries(const Request& req, Response& resp) {
  string start_param = req.get_param_value("start");
  string end_param = req.get_param_value("end");
  Date start{0, 0, 0};
  Date end{0, 0, 0};
  try {
    start = Date::parse(start_param);
    end = Date::parse(end_param);
  } catch (const DateError& e) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content(jsonMsg(e.what()), "application/json");
    return;
  }
  if (start > end) {
    resp.status = HttpStatus::kBadRequest;
    resp.set_content("start can't be greater than end", "application/json");
    SPDLOG_WARN(
        format("GET /api/summaries 400, msg='start({}) can't be greater "
               "than end({})'",
               start.toString(), end.toString()));
    return;
  }

  auto summaries = HeartbeatService{}.summarize(start, end);

  Result result;
  result.total.name = format("'{}'~'{}'", start.toString(), end.toString());
  result.total.time_text = formatTime(summaries.total_msec);
  result.total.total_msec = summaries.total_msec;

  for (auto& i : summaries.categories) {
    Summary s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.categories.array.push_back(std::move(s));
  }

  for (auto& i : summaries.editors) {
    Summary s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.editors.array.push_back(std::move(s));
  }

  for (auto& i : summaries.languages) {
    Summary s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.languages.array.push_back(std::move(s));
  }

  for (auto& i : summaries.oss) {
    Summary s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.oss.array.push_back(std::move(s));
  }

  for (auto& i : summaries.projects) {
    Summary s;
    s.name = std::move(i.first);
    s.time_text = formatTime(i.second);
    s.total_msec = i.second;
    result.projects.array.push_back(std::move(s));
  }

  resp.status = HttpStatus::kOK;
  resp.set_content(result.toJson(), "application/json");
  SPDLOG_INFO("GET /api/summaries 200");
}

}  // namespace waka::controller
