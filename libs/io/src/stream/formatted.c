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

#include <stdio.h>

#include "ecr/error.h"
#include "ecr/stream.h"
#include "ecr/stream/formatted.h"

ecr_status_t ecr_stream_vprintf(ecr_stream_t *stream, const char *format, va_list vlist) {
    int size = vsnprintf(NULL, 0, format, vlist);
    if(size < 0) {
        return ecr_get_system_error();
    }
    if(size == INT_MAX) {
        return ECR_ERROR_TYPE_OVERFLOW;
    }

    char buffer[size + 1];
    int printed = vsnprintf(buffer, sizeof(buffer), format, vlist);
    if(printed < 0) {
        return ecr_get_system_error();
    }
    ecr_assert(printed == size);

    size_t length = (size_t) printed;
    return ecr_stream_write_full(stream, buffer, &length);
}

ecr_status_t ecr_stream_printf(ecr_stream_t *stream, const char *format, ...) {
    va_list vlist;
    va_start(vlist, format);
    ecr_status_t status = ecr_stream_vprintf(stream, format, vlist);

    va_end(vlist);
    return status;
}
