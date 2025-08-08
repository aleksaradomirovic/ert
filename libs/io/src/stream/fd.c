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

#include <limits.h>
#include <stdckdint.h>

#include <unistd.h>

#include "ecr/error.h"
#include "ecr/stream.h"
#include "ecr/stream/fd.h"

#include "posix.h"

static ecr_status_t ecr_stream_fd_readbuf(void *data, ecr_buffer_t *restrict buffer) {
    int fd = *(int *)(&data);

    size_t length = buffer->length - buffer->position;
    if(length <= 0) {
        return ECR_ERROR_FULL_BUFFER;
    }
    if(length > SSIZE_MAX) {
        length = SSIZE_MAX;
    }

    ssize_t read_length = read(fd, buffer->memory + buffer->position, length);
    if(read_length < 0) {
        return ecr_get_system_error();
    }
    if(read_length == 0) {
        return ECR_ERROR_EOF;
    }

    buffer->position += (size_t) read_length;
    return ECR_SUCCESS;
}

static ecr_status_t ecr_stream_fd_writebuf(void *data, ecr_buffer_t *restrict buffer) {
    int fd = *(int *)(&data);

    size_t length = buffer->length - buffer->position;
    if(length <= 0) {
        return ECR_ERROR_FULL_BUFFER;
    }
    if(length > SSIZE_MAX) {
        length = SSIZE_MAX;
    }

    ssize_t write_length = write(fd, buffer->memory + buffer->position, length);
    if(write_length < 0) {
        return ecr_get_system_error();
    }

    buffer->position += (size_t) write_length;
    return ECR_SUCCESS;
}

static ecr_status_t ecr_stream_fd_close(void *data) {
    int fd = *(int *)(&data);
    if(close(fd)) {
        return ecr_get_system_error();
    }

    return ECR_SUCCESS;
}

static ecr_status_t ecr_stream_fd_getpos(void *data, ecr_stream_pos_t *restrict position_ptr) {
    int fd = *(int *)(&data);
    
    off_t offset = lseek(fd, 0, SEEK_CUR);
    if(offset < 0) {
        return ecr_get_system_error();
    }

    ecr_stream_pos_t position;
    if(ckd_add(&position, 0, offset)) {
        return ECR_ERROR_TYPE_OVERFLOW;
    }

    *position_ptr = position;
    return ECR_SUCCESS;
}

static ecr_status_t ecr_stream_fd_setpos(void *data, ecr_stream_pos_t *restrict position_ptr, ecr_stream_dir_t direction) {
    int fd = *(int *)(&data);
    ecr_stream_pos_t position = *position_ptr;

    off_t offset;
    if(direction & ECR_STREAM_DIR_REWIND) {
        if(ckd_sub(&offset, 0, position)) {
            return ECR_ERROR_TYPE_OVERFLOW;
        }
    } else {
        if(ckd_add(&offset, 0, position)) {
            return ECR_ERROR_TYPE_OVERFLOW;
        }
    }

    int whence;
    if(direction & (1 << 1)) {
        if(direction & ECR_STREAM_DIR_REWIND) {
            whence = SEEK_END;
        } else {
            whence = SEEK_SET;
        }
    } else {
        whence = SEEK_CUR;
    }

    off_t result = lseek(fd, offset, whence);
    if(result < 0) {
        return ecr_get_system_error();
    }

    if(ckd_add(&position, 0, result)) {
        return ECR_ERROR_TYPE_OVERFLOW;
    }

    *position_ptr = position;
    return ECR_SUCCESS;
}

void ecr_stream_from_fd_nodup(ecr_stream_t *stream, int fd) {
    stream->version = 0;
    *(int *)(&stream->data) = fd;

    stream->readbuf  = ecr_stream_fd_readbuf;
    stream->writebuf = ecr_stream_fd_writebuf;
    stream->close    = ecr_stream_fd_close;
    stream->getpos   = ecr_stream_fd_getpos;
    stream->setpos   = ecr_stream_fd_setpos;
}

ecr_status_t ecr_stream_from_fd(ecr_stream_t *stream, int fd) {
    fd = dup(fd);
    if(fd < 0) {
        return ecr_get_system_error();
    }

    ecr_stream_from_fd_nodup(stream, fd);
    return ECR_SUCCESS;
}

ecr_stream_t ecr_stdin;
ecr_stream_t ecr_stdout;
ecr_stream_t ecr_stderr;

__attribute__((constructor))
void ert_setup_standard_streams() {
    ecr_stream_from_fd_nodup(&ecr_stdin, STDIN_FILENO);
    ecr_stream_from_fd_nodup(&ecr_stdout, STDOUT_FILENO);
    ecr_stream_from_fd_nodup(&ecr_stderr, STDERR_FILENO);
}
