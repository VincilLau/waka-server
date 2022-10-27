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
version = "0.0.0"
set_version(version)
set_languages("c++17")

add_rules("mode.debug", "mode.release")
add_requires(
    "cli11",
    "cpp-httplib",
    "fmt",
    "gtest",
    "nlohmann_json",
    "spdlog",
    "sqlite3"
)

target("waka-server")
    set_kind("binary")
    add_files("src/main.cpp")
    add_includedirs("src")

    add_defines("SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")
    add_defines("SPDLOG_FMT_EXTERNAL=ON")
    add_defines("CPPHTTPLIB_THREAD_POOL_COUNT=1")
    if is_plat("windows") then
        add_cxxflags("/source-charset:utf-8", "/execution-charset:utf-8")
    end

    add_deps("waka-server_static")
    add_links("waka-server_static")

    add_packages(
        "cli11",
        "cpp-httplib",
        "fmt",
        "nlohmann_json",
        "spdlog",
        "sqlite3"
    )

target("waka-server_static")
    set_kind("static")

    set_configdir("$(projectdir)/src")
    set_configvar("WAKA_VERSION", version)
    set_configvar("WAKA_PROJECT_DIR", "$(projectdir)")
    add_configfiles("src/define.hpp.in")

    add_files("src/common/**.cpp")
    add_files("src/controller/**.cpp")
    add_files("src/dao/**.cpp")
    add_files("src/dto/**.cpp")
    add_files("src/http/**.cpp")
    add_files("src/service/**.cpp")

    add_includedirs("src")

    add_defines("SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")
    add_defines("SPDLOG_FMT_EXTERNAL=ON")

    if is_plat("linux") then
        add_defines("WAKA_PLATFORM=WAKA_PLATFORM_LINUX")
    elseif is_plat("windows") then
        add_defines("WAKA_PLATFORM=WAKA_PLATFORM_WINDOWS")
        add_cxxflags("/source-charset:utf-8", "/execution-charset:utf-8")
    end

    add_packages(
        "cpp-httplib",
        "fmt",
        "nlohmann_json",
        "spdlog",
        "sqlite3"
    )

includes("test")
