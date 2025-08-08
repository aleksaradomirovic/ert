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
link_libraries(ecr-io)

add_executable(
    io_example_helloworld
        helloworld/helloworld.c
)

add_executable(
    io_example_echo
        echo/echo.c
)
