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

#include "heartbeat_service.hpp"

#include <common/pattern.hpp>
#include <common/uuid.hpp>

using std::int64_t;
using std::size_t;
using std::string;
using std::stringstream;
using std::unordered_map;
using waka::bo::Summary;
using waka::common::Date;
using waka::common::genUUIDv4;
using waka::common::MetaData;
using waka::common::parseUserAgent;
using waka::model::Heartbeat;

namespace waka::service {

static string unknownIfEmpty(string str) {
  if (str.empty()) {
    return "Unknown";
  }
  return std::move(str);
}

string HeartbeatService::save(bo::Heartbeat bo) const {
  Heartbeat model;
  model.id = genUUIDv4();
  model.entity = std::move(bo.entity);
  model.time = bo.time;
  model.branch = unknownIfEmpty(std::move(bo.branch));
  model.language = unknownIfEmpty(std::move(bo.language));
  model.project = unknownIfEmpty(std::move(bo.project));
  parseUserAgent(bo.user_agent, model.os, model.editor);

  mapper_.insert(model);
  return model.id;
}

int64_t HeartbeatService::today() const {
  auto lst = mapper_.listByDate(Date::today());
  if (lst.empty()) {
    return 0;
  }

  int timeout = MetaData::getInstance()->timeout;
  // timeout的单位为分钟
  int64_t timeout_msec = timeout * 60 * 1000;
  int64_t msec = 0;
  for (size_t i = 0; i < lst.size() - 1; i++) {
    int64_t duration = lst[i + 1].time - lst[i].time;
    if (duration <= timeout_msec) {
      msec += duration;
    }
  }
  return msec;
}

static void mapIncrease(unordered_map<string, int64_t>& map, const string& key,
                        int64_t value) {
  auto it = map.find(key);
  if (it != map.end()) {
    it->second += value;
  } else {
    map.insert({key, value});
  }
}

Summary HeartbeatService::summarize(const Date& start, const Date& end) {
  assert(start <= end);

  int timeout = MetaData::getInstance()->timeout;
  // timeout的单位为分钟
  int64_t timeout_msec = timeout * 60 * 1000;
  Summary summary;

  for (Date date = start; date <= end; date++) {
    auto lst = mapper_.listByDate(date);
    if (lst.empty()) {
      summary.daily_msec.push_back(0);
      continue;
    }

    int64_t msec_this_day = 0;
    for (size_t i = 0; i < lst.size() - 1; i++) {
      const auto& h1 = lst[i];
      const auto& h2 = lst[i + 1];
      int64_t duration = h2.time - h1.time;
      if (duration > timeout_msec) {
        continue;
      }

      mapIncrease(summary.editors, h1.editor, duration);
      mapIncrease(summary.languages, h1.language, duration);
      mapIncrease(summary.oss, h1.os, duration);
      mapIncrease(summary.projects, h1.project, duration);

      msec_this_day += duration;
    }

    summary.daily_msec.push_back(msec_this_day);
    summary.total_msec += msec_this_day;
  }

  return summary;
}

}  // namespace waka::service
