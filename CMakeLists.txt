# Copyright 2025 Aleksa Radomirovic
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     https://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.31)
project(extensible-c-runtime LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS ON)

option(BUILD_SHARED_LIBS "Build as shared library" ON)
option(BUILD_DOCS "Generate documentation" OFF)
option(BUILD_TESTING "Build tests" OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    set(DOXYGEN_FILE_PATTERNS *.h)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
endif()

if(BUILD_TESTING)
    include(CTest)
endif()

add_compile_definitions(
    _GNU_SOURCE
    _POSIX_C_SOURCE=200809L
)

add_library(
    ecr-core
        src/error.c
)
target_include_directories(
    ecr-core
    PUBLIC
        include
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(libs/io)

if(BUILD_DOCS)
    doxygen_add_docs(
        docs
        include
        libs/io/include
    )
endif()
