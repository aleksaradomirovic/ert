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

#ifndef ECR_BUFFER_H_
#define ECR_BUFFER_H_


#include <stddef.h>

#include <ecr/allocator.h>
#include <ecr/error.h>
#include <ecr/macro/guards.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Struct to represent a buffer for use with I/O operations.
 * @param memory pointer to buffer memory
 * @param capacity size of buffer memory
 * @param position lower positional value
 * @param length higher positional value
 *
 * @note The buffer's **position** must always be less than or equal to its **length**.
 */
typedef struct ecr_buffer {
    void *memory;
    size_t capacity;
    size_t position, length;
} ecr_buffer_t;

/**
 * Free a buffer using an allocator.
 * @param buffer buffer to free
 * @param allocator allocator to use
 * @return status code
 *
 * @see ecr_free
 */
[[maybe_unused]]
static ecr_status_t ecr_buffer_free(ecr_buffer_t *buffer, ecr_allocator_t *allocator) {
    ECR_STATUS_GUARD(ecr_free(allocator, buffer->memory));

    buffer->memory = NULL;
    buffer->capacity = 0;
    buffer->position = 0;
    buffer->length = 0;

    return ECR_SUCCESS;
}

/**
 * Allocate a buffer using an allocator.
 * @param buffer buffer to allocate
 * @param allocator allocator to use
 * @param capacity size of buffer to allocate
 * @return status code
 *
 * @see ecr_allocate
 */
[[maybe_unused]]
static ecr_status_t ecr_buffer_allocate(ecr_buffer_t *buffer, ecr_allocator_t *allocator, size_t capacity) {
    ECR_STATUS_GUARD(ecr_allocate(allocator, &buffer->memory, capacity));

    buffer->capacity = capacity;
    buffer->position = 0;
    buffer->length   = 0;

    return ECR_SUCCESS;
}


#ifdef __cplusplus
}
#endif


#endif
