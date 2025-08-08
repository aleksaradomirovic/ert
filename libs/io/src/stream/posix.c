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

#include <stdlib.h>
#include <unistd.h>

#include <ecr/error.h>
#include <ecr/stream.h>
#include <ecr/stream/fd.h>

static int * ecr_stream_get_fd_ptr(ecr_stream_t *stream) {
    return (int *)(&stream->data);
}

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

ecr_status_t ecr_stream_from_fd(ecr_stream_t *stream, int fd) {
    fd = dup(fd);
    if(fd < 0) {
        return ecr_get_system_error();
    }

    *stream = (ecr_stream_t) {
        .version = 0,

        .readbuf  = ecr_stream_fd_readbuf,
        .writebuf = ecr_stream_fd_writebuf,
        .close    = ecr_stream_fd_close,
    };

    *ecr_stream_get_fd_ptr(stream) = fd;
    return ECR_SUCCESS;
}

ecr_stream_t ecr_stdin;
ecr_stream_t ecr_stdout;
ecr_stream_t ecr_stderr;

__attribute__((constructor))
void ert_setup_standard_streams() {
    if(ecr_stream_from_fd(&ecr_stdin, STDIN_FILENO)) {
        abort();
    }

    if(ecr_stream_from_fd(&ecr_stdout, STDOUT_FILENO)) {
        abort();
    }

    if(ecr_stream_from_fd(&ecr_stderr, STDERR_FILENO)) {
        abort();
    }
}

__attribute__((destructor))
void ert_cleanup_standard_streams() {
    ecr_stream_close(&ecr_stdin);
    ecr_stream_close(&ecr_stdout);
    ecr_stream_close(&ecr_stderr);
}
