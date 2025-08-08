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

#ifndef ECR_ERROR_H_
#define ECR_ERROR_H_


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Type for holding function return status / error codes.
 */
typedef enum : uint_least32_t {
    /// general success status
    ECR_SUCCESS       = 0x0,
    /// error of unspecified type
    ECR_ERROR_UNKNOWN = 0x1,
    /// system error code
    ECR_ERROR_SYSTEM  = 0x2,


    /// \cond
    ECR_ERROR_TYPE_USER_CALL   = 0x10, /// \endcond
    /// operation not supported
    ECR_ERROR_NOT_SUPPORTED    = ECR_ERROR_TYPE_USER_CALL + 0x1,
    /// invalid argument provided
    ECR_ERROR_INVALID_ARGUMENT = ECR_ERROR_TYPE_USER_CALL + 0x2,

    /// \cond
    ECR_ERROR_TYPE_IO     = 0x30, /// \endcond
    /// general i/o error
    ECR_ERROR_IO          = ECR_ERROR_TYPE_IO + 0x0,
    /// end of file error
    ECR_ERROR_EOF         = ECR_ERROR_TYPE_IO + 0x1,
    /// full buffer
    ECR_ERROR_FULL_BUFFER = ECR_ERROR_TYPE_IO + 0x2,
} ecr_status_t;

/**
 * Returns a string for the provided status code.
 *
 * @param status status code to get a string for
 * @return a corresponding string
 */
const char * ecr_status_string(ecr_status_t status);

/**
 * Updates this thread's system error value to the current value of `errno`,
 * and returns a deducible error code if one exists.
 *
 * @return a status code representing the current value of `errno`, or {@link ECR_ERROR_SYSTEM} if it can't be deduced
 */
ecr_status_t ecr_get_system_error();


#ifdef __cplusplus
}
#endif


#endif
