-- Copyright 2022 Vincil Lau
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

set_project("waka-server")
set_version("0.0.0")
set_languages("c++17")

add_rules("mode.debug", "mode.release")
add_requires(
    "cpp-httplib",
    "fmt",
    "gtest",
    "nlohmann_json",
    "spdlog",
    "sqlite"
)

target("waka-server")
    set_kind("binary")
    add_files("src/**.cpp")
    add_includedirs("src")
    add_defines("SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")
    add_defines("SPDLOG_FMT_EXTERNAL=ON")
    add_defines("CPPHTTPLIB_THREAD_POOL_COUNT=1")
    add_packages(
        "cpp-httplib",
        "fmt",
        "nlohmann_json",
        "spdlog",
        "sqlite"
    )
