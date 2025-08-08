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

#ifndef ECR_ALLOCATOR_STANDARD_H_
#define ECR_ALLOCATOR_STANDARD_H_


#include <stdlib.h>

#include <ecr/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Wraps the standard libc `free()` function.
 *
 * @see {@link ecr_allocator_free_fn_t}
 */
[[maybe_unused]]
static ecr_status_t ecr_allocator_standard_free(void *data, void *mem) {
    free(mem);
    return ECR_SUCCESS;
}

/**
 * Wraps the standard libc `malloc()` function.
 *
 * @see {@link ecr_allocator_alloc_fn_t}
 */
[[maybe_unused]]
static ecr_status_t ecr_allocator_standard_alloc(void *data, void **mem_ptr, size_t mem_size) {
    void *mem = malloc(mem_size);
    if(!mem) {
        return ecr_get_system_error();
    }

    *mem_ptr = mem;
    return ECR_SUCCESS;
}

/**
 * Instantiates the standard allocator.
 */
#define ecr_allocator_standard ((ecr_allocator_t) { .version = 0, .data = NULL, .free = ecr_allocator_standard_free, .alloc = ecr_allocator_standard_alloc })


#ifdef __cplusplus
}
#endif


#endif
