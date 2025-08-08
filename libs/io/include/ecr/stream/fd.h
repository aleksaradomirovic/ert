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

#ifndef ECR_STREAM_FD_H_
#define ECR_STREAM_FD_H_


#include <unistd.h>

#include <ecr/error.h>
#include <ecr/stream.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Initialize a stream from a unix file descriptor.
 *
 * @param stream pointer to the stream object to be initialized
 * @param fd file descriptor to initialize from
 *
 * @return error code
 */
ecr_status_t ecr_stream_from_fd(ecr_stream_t *stream, int fd);


#ifdef __cplusplus
}
#endif


#endif
