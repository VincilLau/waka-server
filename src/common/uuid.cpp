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

#include "uuid.hpp"

#include <random>

using std::default_random_engine;
using std::random_device;
using std::size_t;
using std::string;
using std::uint16_t;
using std::uniform_int_distribution;

namespace waka::common {

static constexpr size_t kUUIDLength = 36;

string genUUIDv4() {
  string uuid(kUUIDLength, '-');
  random_device dev;
  default_random_engine engine(dev());
  uniform_int_distribution<uint16_t> uniform_dist(0, UINT16_MAX);

  for (size_t i : {0, 4, 9, 14, 19, 24, 28, 32}) {
    uint16_t u16 = uniform_dist(engine);
    char* bytes = uuid.data() + i;
    char b = 0;
    b = u16 >> 12 & 0xf;
    bytes[0] = (b >= 10) ? (b - 10 + 'a') : (b + '0');
    b = u16 >> 8 & 0xf;
    bytes[1] = (b >= 10) ? (b - 10 + 'a') : (b + '0');
    b = u16 >> 4 & 0xf;
    bytes[2] = (b >= 10) ? (b - 10 + 'a') : (b + '0');
    b = u16 & 0xf;
    bytes[3] = (b >= 10) ? (b - 10 + 'a') : (b + '0');
  }

  return uuid;
}

}  // namespace waka::common
