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

#ifndef WAKA_SRC_MODEL_HEARTBEAT_HPP_
#define WAKA_SRC_MODEL_HEARTBEAT_HPP_

#include <cstdint>
#include <string>

namespace waka::model {

// 客户端产生的心跳
// 按照月份进行分表，表名的格式为'heartbeat_YYYY_MM'
// 主键为id
struct Heartbeat {
  // 当前git分支
  // 不允许为NULL或空，未知则为Unknown
  std::string branch;
  // 编辑器
  // 不允许为NULL或空，未知则为Unknown
  std::string editor;
  // 当前正在编辑的文件
  // 不允许为NULL或空
  std::string entity;
  // 心跳的UUID，主键
  std::string id;
  // 编程语言
  // 不允许为NULL或空，未知则为Unknown
  std::string language;
  // 操作系统
  // 不允许为NULL或空，未知则为Unknown
  std::string os;
  // 正在编辑的项目，一般为编辑器正在打开的目录
  // 不允许为NULL或空，未知则为Unknown
  std::string project;
  // 产生心跳的时间戳，单位毫秒，不允许为NULL
  // 对此字段建立索引
  std::int64_t time;
};

}  // namespace waka::model

#endif  // WAKA_SRC_MODEL_HEARTBEAT_HPP_
