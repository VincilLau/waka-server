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

target("test_common_config")
    set_kind("binary")
    set_group("test")
    add_files(
        "test_config.cpp",
        "$(projectdir)/src/common/*.cpp",
        "$(projectdir)/src/dao/*.cpp"
    )
    add_includedirs("$(projectdir)/src", "$(projectdir)/test")
    add_deps("test_main")
    add_links("test_main")
    add_packages(
        "cpp-httplib",
        "fmt",
        "gtest",
        "nlohmann_json",
        "spdlog",
        "sqlite"
    )

target("test_common_re")
    set_kind("binary")
    set_group("test")
    add_files("test_re.cpp")
    add_includedirs("$(projectdir)/src")
    add_deps("test_main")
    add_links("test_main")
    add_packages("gtest")
