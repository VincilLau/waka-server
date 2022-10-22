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

#include <cctype>
#include <common/pattern.hpp>
#include <common/uuid.hpp>
#include <model/heartbeat.hpp>
#include <unordered_map>

using std::out_of_range;
using std::string;
using std::stringstream;
using std::unordered_map;
using waka::common::parseUserAgent;
using waka::common::uuidV4;
using waka::model::Heartbeat;

namespace waka::service {

static void stringToLower(string str) {
  for (char& ch : str) {
    if (ch >= 'A' && ch <= 'Z') {
      ch = ch - 'A' + 'a';
    }
  }
}

static const unordered_map<string, string> kOSMap = {
    {"aix", "AIX"},              //
    {"android", "Android"},      //
    {"darwin", "macOS"},         //
    {"dragonfly", "DragonFly"},  //
    {"freebsd", "FreeBSD"},      //
    {"hurd", "Hurd"},            //
    {"illumos", "Illumos"},      //
    {"ios", "IOS"},              //
    {"js", "JavaScript"},        //
    {"linux", "Linux"},          //
    {"nacl", "NaCl"},            //
    {"netbsd", "NetBSD"},        //
    {"openbsd", "OpenBSD"},      //
    {"plan9", "Plan9"},          //
    {"solaris", "Solaris"},      //
    {"windows", "Windows"},      //
    {"zos", "Z/OS"},             //
    {"unknown", "Unknown"}};

static const unordered_map<string, string> kEditorMap = {
    {"vscode", "VS Code"},   //
    {"unknown", "Unknown"},  //
};

string HeartbeatService::save(bo::Heartbeat heartbeat) const {
  Heartbeat h;
  h.id = uuidV4();

  h.branch = std::move(heartbeat.branch);
  h.category = std::move(heartbeat.category);
  h.entity = std::move(heartbeat.entity);
  h.language = std::move(heartbeat.language);
  h.project = std::move(heartbeat.project);
  h.type = std::move(heartbeat.type);
  h.time = int64_t(heartbeat.time * 1000);

  auto pair = parseUserAgent(heartbeat.user_agent);
  try {
    h.os = kOSMap.at(pair.first);
  } catch (const out_of_range& e) {
    h.os = pair.first;
  }
  try {
    h.editor = kEditorMap.at(pair.second);
  } catch (const out_of_range& e) {
    h.editor = pair.second;
  }

  mapper_.insert(h);
  return h.id;
}

}  // namespace waka::service
