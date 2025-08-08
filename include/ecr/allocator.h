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

#ifndef ECR_ALLOCATOR_H_
#define ECR_ALLOCATOR_H_


#include <stddef.h>

#include <ecr/error.h>
#include <ecr/version.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Struct to represent a memory allocator.
 * @param version implementation-defined version
 * @param data implementation-defined data pointer
 * @param free see {@link ecr_allocator_free_fn_t}
 * @param alloc see {@link ecr_allocator_alloc_fn_t}
 */
typedef struct ecr_allocator ecr_allocator_t;

/**
 * An allocator function template to free a block of memory.
 * 
 * @param data data pointer belonging to the calling allocator
 * @param mem address of the memory block to free
 * @return status code
 *
 * @note The function is allowed to return successfully even if `mem` wasn't a valid address to free.
 */
typedef ecr_status_t ecr_allocator_free_fn_t(void *data, void *mem);

/**
 * An allocator function template to allocate a block of memory.
 * 
 * @param data data pointer belonging to the calling allocator
 * @param mem_ptr pointer to the pointer which will store the address of the allocated block on success
 * @param mem_size size of the block to allocate
 * @return status code
 */
typedef ecr_status_t ecr_allocator_alloc_fn_t(void *data, void **mem_ptr, size_t mem_size);

struct ecr_allocator {
    ecr_version_t version;
    void *data;

    ecr_allocator_free_fn_t *free;
    ecr_allocator_alloc_fn_t *alloc;
};

/**
 * Free a block of memory using an allocator.
 * @param allocator allocator to use
 * @param mem see {@link ecr_allocator_free_fn_t}
 * @return status code
 *
 * @see ecr_allocator_free_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_free(ecr_allocator_t *allocator, void *mem) {
    return allocator->free(allocator->data, mem);
}

/**
 * Allocate a block of memory using an allocator.
 * @param allocator allocator to use
 * @param mem_ptr see {@link ecr_allocator_alloc_fn_t}
 * @param mem_size see {@link ecr_allocator_alloc_fn_t}
 * @return status code
 *
 * @see ecr_allocator_free_fn_t
 */
[[maybe_unused]]
static ecr_status_t ecr_allocate(ecr_allocator_t *allocator, void **mem_ptr, size_t mem_size) {
    return allocator->alloc(allocator->data, mem_ptr, mem_size);
}


#ifdef __cplusplus
}
#endif


#endif
