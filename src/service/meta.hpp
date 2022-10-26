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

#ifndef WAKA_SRC_SERVICE_META_SERVICE_HPP_
#define WAKA_SRC_SERVICE_META_SERVICE_HPP_

#include <common/meta_data.hpp>
#include <dao/meta.hpp>

namespace waka::service {

class MetaService {
 public:
  // 初始化meta表
  void init() const;
  // 从数据库中读取元数据
  // 会对读取到的元数据进行检查，如果元数据不合法会抛出MetaDataError异常
  [[nodiscard]] common::MetaData readMetaData() const;
  // 向数据库写入元数据
  // 写入前对元数据进行检查，如果元数据不合法会抛出MetaDataError异常
  void writeMetaData(const common::MetaData& meta_data) const;

 private:
  dao::MetaMapper mapper_;
};

}  // namespace waka::service

#endif  // WAKA_SRC_SERVICE_META_SERVICE_HPP_
