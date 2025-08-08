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

#include <fcntl.h>
#include <unistd.h>

#include "ecr/stream/file.h"

#include "posix.h"

void ecr_stream_from_file_nodup(ecr_stream_t *stream, int fd) {
    ecr_stream_from_fd_nodup(stream, fd);
}

ecr_status_t ecr_stream_open_file(ecr_stream_t *stream, const char *pathname, ecr_filemode_t mode_flags) {
    int fcntl_flags = 0;
    ecr_filemode_t access_mode = mode_flags & ECR_FILEMODE_ACCESS_MODE_RDWR_MASK;
    switch(access_mode) {
        case ECR_FILEMODE_READ_ONLY:
            fcntl_flags |= O_RDONLY;
            break;
        case ECR_FILEMODE_WRITE_ONLY:
            fcntl_flags |= O_WRONLY;
            break;
        case ECR_FILEMODE_READ_WRITE:
            fcntl_flags |= O_RDWR;
            break;
        default:
            return ECR_ERROR_INVALID_ARGUMENT;
    }

    if(mode_flags & ECR_FILEMODE_APPEND) {
        if(!(access_mode & ECR_FILEMODE_WRITE_ONLY)) {
            return ECR_ERROR_INVALID_ARGUMENT;
        }
        fcntl_flags |= O_APPEND;
    }

    if(mode_flags & ECR_FILEMODE_CREATE) {
        fcntl_flags |= O_CREAT;
    }

    int fd;
    if(fcntl_flags & O_CREAT) {
        fd = open(pathname, fcntl_flags, 0600);
    } else {
        fd = open(pathname, fcntl_flags);
    }
    if(fd < 0) {
        return ecr_get_system_error();
    }

    ecr_stream_from_file_nodup(stream, fd);
    return ECR_SUCCESS;
}
