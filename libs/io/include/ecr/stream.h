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

#ifndef ECR_STREAM_H_
#define ECR_STREAM_H_


#include <ecr/buffer.h>
#include <ecr/error.h>
#include <ecr/macro/assume.h>
#include <ecr/macro/guards.h>
#include <ecr/version.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Type for holding the value of a position in a stream.
 */
typedef uint_least64_t ecr_stream_pos_t;

/**
 * Type for defining a directional mode for a {@link ecr_stream_pos_t}.
 */
typedef enum {
    /// skip forward in the stream
    ECR_STREAM_DIR_SKIP   = (0 << 0),
    /// rewind backward in the stream
    ECR_STREAM_DIR_REWIND = (1 << 0),

    /// skip starting from the beginning of the stream
    ECR_STREAM_DIR_START  = (1 << 1) | ECR_STREAM_DIR_SKIP,
    /// rewind starting from the end of the stream
    ECR_STREAM_DIR_END    = (1 << 1) | ECR_STREAM_DIR_REWIND,
} ecr_stream_dir_t;

/**
 * Struct to represent an I/O stream.
 * @param version implementation-defined version
 * @param data implementation-defined data pointer
 * @param readbuf see {@link ecr_stream_readbuf_fn_t}
 * @param writebuf see {@link ecr_stream_writebuf_fn_t}
 * @param close see {@link ecr_stream_close_fn_t}
 */
typedef struct ecr_stream ecr_stream_t;

/**
 * A stream function template to read into a **buffer**,
 * whose position value is advanced by the number of bytes read.
 *
 * @param data data pointer belonging to the stream
 * @param buffer buffer to read into
 *
 * @return error code
 *
 * @note The function may ONLY affect the buffer's position value and the contents of its memory block.
 */
typedef ecr_status_t ecr_stream_readbuf_fn_t(void *data, ecr_buffer_t *restrict buffer);

/**
 * A stream function template to write from a **buffer**,
 * whose position value is advanced by the number of bytes written.
 *
 * @param data data pointer belonging to the stream
 * @param buffer buffer to write from
 *
 * @return error code
 *
 * @note The function may ONLY affect the buffer's position value. It MUST NOT affect the contents of its memory block.
 */
typedef ecr_status_t ecr_stream_writebuf_fn_t(void *data, ecr_buffer_t *restrict buffer);

/**
 * A stream function template to close said **stream**.
 *
 * @param data data pointer belonging to the stream
 *
 * @return error code
 */
typedef ecr_status_t ecr_stream_close_fn_t(void *data);

/**
 * A stream function template to learn a stream's positon.
 *
 * @param data data pointer belonging to the stream
 * @param position pointer to the position value to be returned
 *
 * @return error code
 */
typedef ecr_status_t ecr_stream_getpos_fn_t(void *data, ecr_stream_pos_t *restrict position);

/**
 * A stream function template to set/move a stream's positon.
 *
 * @param data data pointer belonging to the stream
 * @param position pointer to the position value to be read;
 * it will contain the stream's position after a successful return as if
 * {@link ecr_stream_getpos_fn_t} were called.
 * @param direction direction to set/move in
 *
 * @return error code
 */
typedef ecr_status_t ecr_stream_setpos_fn_t(void *data, ecr_stream_pos_t *restrict position, ecr_stream_dir_t direction);

struct ecr_stream {
    ecr_version_t version;
    void *data;

    ecr_stream_readbuf_fn_t *readbuf;
    ecr_stream_writebuf_fn_t *writebuf;

    ecr_stream_close_fn_t *close;

    ecr_stream_getpos_fn_t *getpos;
    ecr_stream_setpos_fn_t *setpos;
};

/**
 * Read from a stream into a buffer,
 * whose position value is advanced by the number of bytes read.
 *
 * @param stream stream to read from
 * @param buffer buffer to read into
 *
 * @return error code
 *
 * @see ecr_stream_readbuf_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_readbuf(ecr_stream_t *stream, ecr_buffer_t *restrict buffer) {
    void *memory = buffer->memory;
    size_t capacity = buffer->capacity;
    size_t length = buffer->length;

    ecr_status_t status = stream->readbuf(stream->data, buffer);

    ecr_assert(buffer->memory == memory);
    ecr_assert(buffer->capacity == capacity);
    ecr_assert(buffer->length == length);

    return status;
}

/**
 * Read up to `*length` bytes from a stream into a memory block.
 * 
 * @param stream stream to read from
 * @param memory memory block to read into
 * @param length pointer to a value that initially holds the length of the memory block,
 * and on successful return will hold the number of bytes read
 *
 * @return error code
 *
 * @see ecr_stream_readbuf
 * @see ecr_stream_readbuf_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_read(ecr_stream_t *stream, void *memory, size_t *restrict length) {
    ecr_buffer_t buffer = {
        .memory   = memory,
        .capacity = *length,
        .position = 0,
        .length   = *length,
    };

    ecr_status_t status = ecr_stream_readbuf(stream, &buffer);

    *length = buffer.position;
    return status;
}

/**
 * Read from a stream into a buffer until the buffer is full,
 * advancing the buffer's position by the number of bytes read.
 *
 * @param stream stream to read from
 * @param buffer buffer to read into
 *
 * @return error code
 *
 * @see ecr_stream_readbuf
 * @see ecr_stream_readbuf_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_readbuf_full(ecr_stream_t *stream, ecr_buffer_t *restrict buffer) {
    if(buffer->length - buffer->position == 0) {
        return ECR_ERROR_FULL_BUFFER;
    }

    while(buffer->length - buffer->position > 0) {
        ECR_STATUS_GUARD(ecr_stream_readbuf(stream, buffer));
    }

    return ECR_SUCCESS;
}

/**
 * Read from a stream into a memory block until the memory block is full,
 * advancing the number of bytes read in `*length`.
 *
 * @param stream stream to read from
 * @param memory memory block to read into
 * @param length pointer to a value that initially holds the length of the memory block,
 * and on successful return will hold the number of bytes read
 *
 * @return error code
 *
 * @see ecr_stream_readbuf_full
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_read_full(ecr_stream_t *stream, void *memory, size_t *restrict length) {
    ecr_buffer_t buffer = {
        .memory   = memory,
        .capacity = *length,
        .position = 0,
        .length   = *length,
    };

    ecr_status_t status = ecr_stream_readbuf_full(stream, &buffer);

    *length = buffer.position;
    return status;
}

/**
 * Write into a stream from a buffer,
 * whose position value is advanced by the number of bytes written.
 *
 * @param stream stream to write into
 * @param buffer buffer to write from
 *
 * @return error code
 *
 * @see ecr_stream_writebuf_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_writebuf(ecr_stream_t *stream, ecr_buffer_t *restrict buffer) {
    void *memory = buffer->memory;
    size_t capacity = buffer->capacity;
    size_t length = buffer->length;

    ecr_status_t status = stream->writebuf(stream->data, buffer);

    ecr_assert(buffer->memory == memory);
    ecr_assert(buffer->capacity == capacity);
    ecr_assert(buffer->length == length);

    return status;
}

/**
 * Write up to `*length` bytes into a stream from a memory block.
 * 
 * @param stream stream to write into
 * @param memory memory block to write from
 * @param length pointer to a value that initially holds the length of the memory block,
 * and on successful return will hold the number of bytes written
 *
 * @return error code
 *
 * @see ecr_stream_writebuf
 * @see ecr_stream_writebuf_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_write(ecr_stream_t *stream, void *memory, size_t *restrict length) {
    ecr_buffer_t buffer = {
        .memory   = memory,
        .capacity = *length,
        .position = 0,
        .length   = *length,
    };

    ecr_status_t status = ecr_stream_writebuf(stream, &buffer);

    *length = buffer.position;
    return status;
}

/**
 * Write into a stream from a buffer until the buffer is fully written,
 * advancing the buffer's position by the number of bytes written.
 *
 * @param stream stream to write into
 * @param buffer buffer to write from
 *
 * @return error code
 *
 * @see ecr_stream_writebuf
 * @see ecr_stream_writebuf_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_writebuf_full(ecr_stream_t *stream, ecr_buffer_t *restrict buffer) {
    if(buffer->length - buffer->position == 0) {
        return ECR_ERROR_FULL_BUFFER;
    }

    while(buffer->length - buffer->position > 0) {
        ECR_STATUS_GUARD(ecr_stream_writebuf(stream, buffer));
    }

    return ECR_SUCCESS;
}

/**
 * Write into a stream from a memory block until the memory block is fully written,
 * advancing the number of bytes written in `*length`.
 *
 * @param stream stream to write into
 * @param memory memory block to write from
 * @param length pointer to a value that initially holds the length of the memory block,
 * and on successful return will hold the number of bytes written
 *
 * @return error code
 *
 * @see ecr_stream_writebuf_full
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_write_full(ecr_stream_t *stream, void *memory, size_t *restrict length) {
    ecr_buffer_t buffer = {
        .memory   = memory,
        .capacity = *length,
        .position = 0,
        .length   = *length,
    };

    ecr_status_t status = ecr_stream_writebuf_full(stream, &buffer);

    *length = buffer.position;
    return status;
}

/**
 * Close a stream.
 *
 * @param stream stream to close
 *
 * @return error code
 *
 * @see ecr_stream_close_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_close(ecr_stream_t *stream) {
    return stream->close(stream->data);
}

/**
 * Learn a stream's positon.
 *
 * @param stream stream to query for position
 * @param position pointer to the position value to be returned
 *
 * @return error code
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_getpos(ecr_stream_t *stream, ecr_stream_pos_t *restrict position) {
    return stream->getpos(stream->data, position);
}

/**
 * Set/move a stream's positon.
 *
 * @param stream stream to set position for
 * @param position pointer to the position value to be read;
 * it will contain the stream's position after a successful return as if
 * {@link ecr_stream_getpos} were called.
 * @param direction direction to set/move in
 *
 * @return error code
 */
[[maybe_unused]]
static ecr_status_t ecr_stream_setpos(ecr_stream_t *stream, ecr_stream_pos_t *restrict position, ecr_stream_dir_t direction) {
    return stream->setpos(stream->data, position, direction);
}

/// Stream corresponding to standard input.
extern ecr_stream_t ecr_stdin;

/// Stream corresponding to standard output.
extern ecr_stream_t ecr_stdout;

/// Stream corresponding to standard error.
extern ecr_stream_t ecr_stderr;


#ifdef __cplusplus
}
#endif


#endif
