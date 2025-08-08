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

#ifndef ECR_STREAM_FORMATTED_H_
#define ECR_STREAM_FORMATTED_H_


#include <stdarg.h>

#include <ecr/error.h>
#include <ecr/stream.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Print formatted text to the specified stream in the same formatting conventions as the
 * `printf()` family of functions.
 *
 * @param stream stream to print to
 * @param format format string
 * @param ... formatted arguments
 *
 * @return error code
 */
ecr_status_t ecr_stream_printf(ecr_stream_t *stream, const char *format, ...);

/**
 * Print formatted text to the specified stream in the same formatting conventions as the
 * `printf()` family of functions.
 *
 * @param stream stream to print to
 * @param format format string
 * @param vlist formatted arguments
 *
 * @return error code
 */
ecr_status_t ecr_stream_vprintf(ecr_stream_t *stream, const char *format, va_list vlist);


#ifdef __cplusplus
}
#endif


#endif
