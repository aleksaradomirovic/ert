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

#ifndef ECR_STREAM_FILE_H_
#define ECR_STREAM_FILE_H_


#include <ecr/error.h>
#include <ecr/stream.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Type for defining a file access mode.
 */
typedef enum : uint_least32_t {
    /// \cond
    ECR_FILEMODE_ACCESS_MODE_MASK = (0xff << 0), /// \endcond
    /// \cond
    ECR_FILEMODE_ACCESS_MODE_RDWR_MASK = (0b11 << 0), /// \endcond

    /// Read-only access mode
    ECR_FILEMODE_READ_ONLY  = (1 << 0),
    /// Write-only access mode
    ECR_FILEMODE_WRITE_ONLY = (1 << 1),
    /// Read-write access mode
    ECR_FILEMODE_READ_WRITE = (ECR_FILEMODE_READ_ONLY | ECR_FILEMODE_WRITE_ONLY),
    /// Append on write
    ECR_FILEMODE_APPEND     = (1 << 2),

    /// Create file if nonexistent
    ECR_FILEMODE_CREATE     = (1 << 8),
} ecr_filemode_t;

/**
 * Open a file as a stream.
 *
 * @param stream pointer to the stream object to be initialized
 * @param pathname file path name to open
 * @param mode_flags file access mode flags
 *
 * @note the access mode MUST contain at least one of the following:
 * * {@link ECR_FILEMODE_READ_ONLY}
 * * {@link ECR_FILEMODE_WRITE_ONLY}
 * * {@link ECR_FILEMODE_READ_WRITE}
 *
 * @return error code
 * * {@link ECR_ERROR_INVALID_ARGUMENT} for an invalid access mode
 * * {@link ECR_ERROR_INVALID_ARGUMENT} if {@link ECR_FILEMODE_APPEND} is specified but access mode is not write-enabled
 */
ecr_status_t ecr_stream_open_file(ecr_stream_t *stream, const char *pathname, ecr_filemode_t mode_flags);


#ifdef __cplusplus
}
#endif


#endif
