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

#ifndef ECR_MACRO_GUARDS_H_
#define ECR_MACRO_GUARDS_H_


#include <ecr/error.h>
#include <ecr/version.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returns {@link ECR_ERROR_NOT_SUPPORTED} from the current function if **version** is below the expected **minimum**.
 */
#define ECR_VERSION_GUARD(version, minimum) { if(version < minimum) { return ECR_ERROR_NOT_SUPPORTED; } }

/**
 * Returns **status** immediately from the current function if **status** is non-zero, calling the provided **destructor** block first.
 *
 * @note This macro should never be present in a destructor itself. Doing so will result in confusing compilation errors and little else.
 */
#define ECR_STATUS_GUARD_WITH_DESTRUCTOR(status, destructor) { ecr_status_t ecr_status__ = status; if(ecr_status__) { { destructor; }; return ecr_status__; } }

/**
 * Convenience macro for a {@link ECR_STATUS_GUARD_WITH_DESTRUCTOR} with no destructor.
 *
 * @see ECR_STATUS_GUARD_WITH_DESTRUCTOR
 */
#define ECR_STATUS_GUARD(status) ECR_STATUS_GUARD_WITH_DESTRUCTOR(status, {})


#ifdef __cplusplus
}
#endif


#endif
