/*
 * Copyright 2025 Aleksa Radomirovic
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     https://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ecr/stream/fd.h>

int main() {
    ecr_stream_t stream_in, stream_out;
    if(ecr_stream_from_fd(&stream_in, STDIN_FILENO)) {
        return 1;
    }

    if(ecr_stream_from_fd(&stream_out, STDOUT_FILENO)) {
        return 2;
    }

    char inbuf[8192];
    size_t length = sizeof(inbuf);
    if(ecr_stream_read(&stream_in, inbuf, &length)) {
        return 3;
    }

    if(ecr_stream_write_full(&stream_out, inbuf, &length)) {
        return 4;
    }

    if(ecr_stream_close(&stream_out)) {
        return 5;
    }

    if(ecr_stream_close(&stream_in)) {
        return 6;
    }

    return 0;
}
