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

#ifndef WAKA_SRC_CONTROLLER_WRAPPER_HPP_
#define WAKA_SRC_CONTROLLER_WRAPPER_HPP_

#include <fmt/core.h>
#include <httplib.h>

#include <common/http.hpp>
#include <functional>
#include <string>

namespace waka::controller {

using Handler =
    std::function<void(const httplib::Request&, httplib::Response&)>;

inline static Handler wrapper(Handler handler) {
  return [handler = std::move(handler)](const httplib::Request&,
                                        httplib::Response& resp) {
    try {
    } catch (const std::exception& e) {
      resp.status = common::HttpStatus::kInternalServerError;
      resp.set_content(fmt::format(R"({{"message":"{}"}})", e.what()),
                       "application/json");
    }
  };
}

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_WRAPPER_HPP_
