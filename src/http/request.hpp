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

#ifndef WAKA_SRC_HTTP_REQUEST_HPP_
#define WAKA_SRC_HTTP_REQUEST_HPP_

#include <httplib.h>

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace waka::http {

class Request {
 public:
  [[nodiscard]] virtual const char* getMethod() const = 0;
  [[nodiscard]] virtual const char* getPath() const = 0;
  [[nodiscard]] virtual std::size_t getParamCount(const char* key) const = 0;
  [[nodiscard]] virtual std::string getParam(const char* key,
                                             std::size_t index = 0) const = 0;
  [[nodiscard]] virtual std::size_t getHeaderCount(const char* key) const = 0;
  [[nodiscard]] virtual std::string getHeader(const char* key,
                                              std::size_t index = 0) const = 0;
  [[nodiscard]] virtual const std::string& getBody() const = 0;
};

class HTTPLibRequest : public Request {
 public:
  HTTPLibRequest(const httplib::Request& req) : req_(req) {}

  [[nodiscard]] const char* getMethod() const override {
    return req_.path.c_str();
  }
  [[nodiscard]] const char* getPath() const override {
    return req_.path.c_str();
  }
  [[nodiscard]] std::size_t getParamCount(const char* key) const override {
    return req_.get_param_value_count(key);
  }
  [[nodiscard]] std::string getParam(const char* key,
                                     std::size_t index = 0) const override {
    return req_.get_param_value(key, index);
  }
  [[nodiscard]] std::size_t getHeaderCount(const char* key) const override {
    return req_.get_header_value_count(key);
  }
  [[nodiscard]] std::string getHeader(const char* key,
                                      std::size_t index = 0) const override {
    return req_.get_header_value(key, index);
  }
  [[nodiscard]] const std::string& getBody() const override {
    return req_.body;
  }

 private:
  const httplib::Request& req_;
};

}  // namespace waka::http

#endif  // WAKA_SRC_HTTP_REQUEST_HPP_
