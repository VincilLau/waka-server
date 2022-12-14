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

#ifndef WAKA_SRC_EXCEPTION_META_DATA_ERROR_HPP_
#define WAKA_SRC_EXCEPTION_META_DATA_ERROR_HPP_

#include <fmt/core.h>

#include "exception.hpp"

namespace waka::exception {

// 无效的元数据
class MetaDataError : public Exception {
 public:
  MetaDataError(const std::string& key, const std::string& value)
      : Exception(
            fmt::format("invalid meta (key='{}', value='{}')", key, value)) {}
};

}  // namespace waka::exception

#endif  // WAKA_SRC_EXCEPTION_META_DATA_ERROR_HPP_
