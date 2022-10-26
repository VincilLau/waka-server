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

#ifndef WAKA_SRC_CONTROLLER_CONFIG_HPP_
#define WAKA_SRC_CONTROLLER_CONFIG_HPP_

#include <dto/config/put.hpp>
#include <http/http.hpp>
#include <service/meta.hpp>

namespace waka::controller {

class ConfigController {
 public:
  // GET /api/config
  void get(const http::Request& rep, http::Response& resp);
  // PUT /api/config
  void put(const http::Request& rep, http::Response& resp);

 private:
  service::MetaService meta_service_;
};

}  // namespace waka::controller

#endif  // WAKA_SRC_CONTROLLER_CONFIG_HPP_
