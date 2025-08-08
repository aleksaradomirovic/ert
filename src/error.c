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

#include <errno.h>
#include <string.h>

#include "ecr/error.h"

#define UNKNOWN_STATUS_STRING "unknown status code"

static thread_local int syserrnum = 0;

static const char * ecr_status_string_system(int errnum) {
    if(errnum == 0) {
        return UNKNOWN_STATUS_STRING;
    }

    const char *errstr;
# if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 32)
    errstr = strerrordesc_np(errnum);
# else
    errstr = strerror(errnum);
# endif
    if(!errstr) {
        errstr = UNKNOWN_STATUS_STRING;
    }

    return errstr;
}

static const char * ecr_status_string_special(ecr_status_t status) {
    switch(status) {
        case ECR_SUCCESS:
            return "success";
        case ECR_ERROR_SYSTEM:
            return ecr_status_string_system(syserrnum);

        case ECR_ERROR_UNKNOWN:
        default:
            return UNKNOWN_STATUS_STRING;
    }
}

const char * ecr_status_string(ecr_status_t status) {
    if(status < 0x10) {
        return ecr_status_string_special(status);
    }

    switch(status) {
        case ECR_ERROR_NOT_SUPPORTED:
            return "operation not supported";
        case ECR_ERROR_INVALID_ARGUMENT:
            return "invalid argument provided";
        
        case ECR_ERROR_IO:
            return "i/o error";
        case ECR_ERROR_EOF:
            return "end of stream reached";

        default:
            return UNKNOWN_STATUS_STRING;
    }
}

ecr_status_t ecr_get_system_error() {
    syserrnum = errno;

    switch(syserrnum) {
        case EINVAL:
            return ECR_ERROR_INVALID_ARGUMENT;
        case ENOTSUP:
            return ECR_ERROR_NOT_SUPPORTED;
        case EIO:
            return ECR_ERROR_IO;
        case ENOBUFS:
            return ECR_ERROR_FULL_BUFFER;
    }

    return ECR_ERROR_SYSTEM;
}
