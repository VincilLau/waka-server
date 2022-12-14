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

#ifndef WAKA_SRC_HTTP_RESPONSE_HPP_
#define WAKA_SRC_HTTP_RESPONSE_HPP_

#include <httplib.h>

#include <string>

#include "status.hpp"

namespace waka::http {

class Response {
 public:
  virtual void setStatus(Status status) = 0;
  virtual void addHeader(const char* key, const char* value) = 0;
  virtual void setContent(const std::string& data, const char* mime_type) = 0;
  virtual void setRedirect(const char* url) = 0;
};

class HTTPLibResponse : public Response {
 public:
  HTTPLibResponse(httplib::Response& resp) : resp_(resp) {}

  void setStatus(Status status) override { resp_.status = status; }
  void addHeader(const char* key, const char* value) override {
    resp_.set_header(key, value);
  }
  void setContent(const std::string& data, const char* mime_type) override {
    resp_.set_content(data, mime_type);
  }
  void setRedirect(const char* url) override { resp_.set_redirect(url); }

 private:
  httplib::Response& resp_;
};

}  // namespace waka::http

#endif  // WAKA_SRC_HTTP_RESPONSE_HPP_
