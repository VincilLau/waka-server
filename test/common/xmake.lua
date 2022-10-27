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

target("test_common_uuid")
    set_kind("binary")
    set_group("test")

    add_files("test_uuid.cpp")
    add_includedirs("$(projectdir)/src")

    if is_plat("windows") then
        add_cxxflags("/source-charset:utf-8", "/execution-charset:utf-8")
    end

    add_deps("waka-server_static")
    add_links("waka-server_static")

    add_packages("gtest", "spdlog")

target("test_common_date")
    set_kind("binary")
    set_group("test")

    add_files("test_date.cpp")
    add_includedirs("$(projectdir)/src")

    if is_plat("windows") then
        add_cxxflags("/source-charset:utf-8", "/execution-charset:utf-8")
    end

    add_deps("waka-server_static")
    add_links("waka-server_static")

    add_packages("fmt", "gtest", "spdlog")
