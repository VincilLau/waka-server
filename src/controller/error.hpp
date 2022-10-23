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

#ifndef WAKA_SRC_CONTROLLER_ERROR_HPP_
#define WAKA_SRC_CONTROLLER_ERROR_HPP_

#include <fmt/core.h>
#include <httplib.h>

#include <common/http.hpp>

namespace waka::controller {

inline static void notFound(const httplib::Request& req,
                            httplib::Response& resp) {
  resp.status = common::HttpStatus::kNotFound;
  resp.set_content(R"({"message":"not found"})", "application/json");
}

inline static void methodNotAllowed(const httplib::Request& req,
                                    httplib::Response& resp) {
  resp.status = common::HttpStatus::kMethodNotAllowed;
  resp.set_content(R"({"message":"method not allowed"})", "application/json");
}

inline static void exceptionHandler(const httplib::Request& req,
                                    httplib::Response& resp, std::exception e) {
  resp.status = common::HttpStatus::kInternalServerError;
  resp.set_content(fmt::format(R"({{"message":"{}"}})", e.what()),
                   "application/json");
}

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_ERROR_HPP_
