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

#include <spdlog/spdlog.h>

#include <http/http.hpp>
#include <stdexcept>

#include "msg.hpp"

namespace waka::controller {

template <typename Controller, http::Method Method>
class Wrapper {};

template <typename Controller>
class Wrapper<Controller, http::GET> {
  void operator()(const http::Request& req, http::Response& resp) {
    try {
      Controller{}.get(req, resp);
    } catch (const std::exception& e) {
      SPDLOG_WARN("{} {} -- {}", req.getMethod(), req.getPath(), e.what());
      resp.setStatus(http::Status::kInternalServerError);
      resp.setContent(jsonMsg(e.what()), "application");
    }
  }
};

template <typename Controller>
class Wrapper<Controller, http::POST> {
  void operator()(const http::Request& req, http::Response& resp) {
    try {
      Controller{}.post(req, resp);
    } catch (const std::exception& e) {
      SPDLOG_WARN("{} {} -- {}", req.getMethod(), req.getPath(), e.what());
      resp.setStatus(http::Status::kInternalServerError);
      resp.setContent(jsonMsg(e.what()), "application");
    }
  }
};

template <typename Controller>
class Wrapper<Controller, http::PUT> {
  void operator()(const http::Request& req, http::Response& resp) {
    try {
      Controller{}.put(req, resp);
    } catch (const std::exception& e) {
      SPDLOG_WARN("{} {} -- {}", req.getMethod(), req.getPath(), e.what());
      resp.setStatus(http::Status::kInternalServerError);
      resp.setContent(jsonMsg(e.what()), "application");
    }
  }
};

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_WRAPPER_HPP_
