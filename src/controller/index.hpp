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

#ifndef WAKA_SRC_CONTROLLER_INDEX_HPP_
#define WAKA_SRC_CONTROLLER_INDEX_HPP_

#include <http/http.hpp>

namespace waka::controller {

class IndexController {
 public:
  // GET /
  void get(const http::Request& req, http::Response& resp) {
    resp.setRedirect("/dashboard");
  }
};

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_INDEX_HPP_
