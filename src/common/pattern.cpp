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

#include "pattern.hpp"

#include <regex>
#include <unordered_map>

#include "meta_data.hpp"

using fmt::format;
using std::regex;
using std::regex_match;
using std::regex_replace;
using std::smatch;
using std::string;
using std::to_string;
using std::unordered_map;

namespace waka::common {

bool isValidIP(const string& ip) {
  regex pattern{
      "^"
      "((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})"
      "(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2}))"
      "{3}"
      "$"};
  return regex_match(ip, pattern);
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
};

static const unordered_map<string, string> kEditorMap = {
    {"vscode", "VS Code"},
};

void parseUserAgent(const string& ua, string& os, string& editor) {
  os = editor = "Unknown";

  regex pattern(
      "^wakatime/.+? "
      "\\((.+?)-.+\\) "
      ".+? "
      "(.+?)/.+?");
  smatch matches;
  if (regex_match(ua, matches, pattern)) {
    assert(matches.size() == 3);
    os = matches[1];
    editor = matches[2];
  }

  auto os_iter = kOSMap.find(os);
  if (os_iter != kOSMap.end()) {
    os = os_iter->second;
  }

  auto editor_iter = kEditorMap.find(editor);
  if (editor_iter != kEditorMap.end()) {
    editor = editor_iter->second;
  }
}

std::string formatTime(std::int64_t msec) {
  assert(msec >= 0);

  int hours = msec / 1000 / 3600;
  int mins = msec / 1000 / 60 % 60;

  auto meta_data = MetaData::getInstance();
  string text = regex_replace(meta_data->time_format, regex{"(%HH)"},
                              format("{:02d}", hours));
  text = regex_replace(text, regex{"(%H)"}, to_string(hours));
  text = regex_replace(text, regex{"(%MM)"}, format("{:02d}", mins));
  text = regex_replace(text, regex{"(%M)"}, to_string(mins));
  return text;
}

}  // namespace waka::common
