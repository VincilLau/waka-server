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

#include "post.hpp"

using fmt::format;
using nlohmann::json;
using std::make_shared;
using std::size_t;
using std::string;
using std::vector;
using waka::exception::JSONError;

namespace waka::dto::heartbeat::post {

static const char* typeName(json::value_t type) {
  switch (type) {
    case json::value_t::null:
      return "null";
    case json::value_t::object:
      return "object";
    case json::value_t::array:
      return "array";
    case json::value_t::string:
      return "string";
    case json::value_t::boolean:
      return "boolean";
    case json::value_t::binary:
      return "binary";
    case json::value_t::discarded:
      return "discarded";
    case json::value_t::number_integer:
    case json::value_t::number_unsigned:
    case json::value_t::number_float:
    default:
      return "number";
  }
}

static void checkType(const json& j, json::value_t type, const string& field) {
  if (j.is_null()) {
    return;
  }
  if (j.type() != type) {
    throw JSONError(format("{} must be a {}, not {}", field, typeName(type),
                           j.type_name()));
  }
}

static Heartbeat heartbeatFromJSON(const json& j, size_t index) {
  const auto& branch = j["branch"];
  const auto& category = j["category"];
  const auto& cursorpos = j["cursorpos"];
  const auto& dependencies = j["dependencies"];
  const auto& entity = j["entity"];
  const auto& type = j["type"];
  const auto& is_write = j["is_write"];
  const auto& language = j["language"];
  const auto& lineno = j["lineno"];
  const auto& lines = j["lines"];
  const auto& project = j["project"];
  const auto& time = j["time"];
  const auto& user_agent = j["user_agent"];

  checkType(branch, json::value_t::string, format("[{}]branch", index));
  checkType(category, json::value_t::string, format("[{}]category", index));
  checkType(cursorpos, json::value_t::number_integer,
            format("[{}]cursorpos", index));
  checkType(dependencies, json::value_t::array,
            format("[{}]dependencies", index));
  checkType(entity, json::value_t::string, format("[{}]entity", index));
  checkType(type, json::value_t::string, format("[{}]type", index));
  checkType(is_write, json::value_t::boolean, format("[{}]is_write", index));
  checkType(language, json::value_t::string, format("[{}]language", index));
  checkType(lineno, json::value_t::number_integer, format("[{}]lineno", index));
  checkType(lines, json::value_t::number_integer, format("[{}]lines", index));
  checkType(project, json::value_t::string, format("[{}]project", index));
  checkType(time, json::value_t::number_float, format("[{}]time", index));
  checkType(user_agent, json::value_t::string, format("[{}]user_agent", index));

  for (size_t i = 0; i < dependencies.size(); i++) {
    const auto& d = dependencies[i];
    checkType(d, json::value_t::string,
              format("[{}]dependencies[{}]", index, i));
  }

  Heartbeat h;
  h.branch = make_shared<string>(branch);
  h.category = make_shared<string>(category);
  h.cursorpos = make_shared<int>(cursorpos);
  h.dependencies = make_shared<vector<string>>(dependencies);
  h.entity = make_shared<string>(entity);
  h.type = make_shared<string>(type);
  h.is_write = make_shared<bool>(is_write);
  h.language = make_shared<string>(language);
  h.lineno = make_shared<int>(lineno);
  h.lines = make_shared<int>(lines);
  h.project = make_shared<string>(project);
  h.time = make_shared<double>(time);
  h.user_agent = make_shared<string>(user_agent);
  return h;
}

Param Param::fromJSON(const string& json_str) {
  Param param;
  json j;
  try {
    j = json::parse(json_str);
  } catch (const json::parse_error& e) {
    throw JSONError(e.what());
  }

  if (j.is_null()) {
    throw JSONError("param can't be null");
  } else if (!j.is_array()) {
    throw JSONError(format("param must be a array, not {}", j.type_name()));
  }

  for (size_t i = 0; i < j.size(); i++) {
    const auto& item = j[i];
    checkType(item, json::value_t::object, format("[{}]", i));
    param.heartbeats.push_back(heartbeatFromJSON(item, i));
  }

  return param;
}

json Result::toJSON() const {
  json::object_t j;
  json::array_t resps;
  for (const auto& resp : responses) {
    json::array_t pair;
    pair.push_back({
        {"data", {{"id", resp.first}}},
    });
    pair.push_back(resp.second);
    resps.push_back(std::move(pair));
  }
  j["responses"] = std::move(resps);
  return static_cast<json>(j);
}

}  // namespace waka::dto::heartbeat::post
