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

set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)
link_libraries(ecr-core)

add_executable(
    allocator_test
        allocator/standard_allocator_test.cpp
)
target_link_libraries(
    allocator_test
        GTest::gtest
        GTest::gtest_main
)
gtest_discover_tests(allocator_test)
