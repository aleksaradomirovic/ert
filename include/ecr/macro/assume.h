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

#ifndef ECR_MACRO_ASSUME_H_
#define ECR_MACRO_ASSUME_H_


#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Potentially hints to the compiler that a conforming program would always evaluate
 * this **expression** as true here. 
 */
#ifdef __clang__
#   define ecr_assume(expression) { __builtin_assume(expression); }
#elifdef __glibc__
#   define ecr_assume(expression) { if(!(expression)) { __builtin_unreachable(); } }
#else
#   define ecr_assume(expression)
#endif

/**
 * Calls assert() and ert_assume().
 */
#define ecr_assert(expression) { assert(expression); ecr_assume(expression); }


#ifdef __cplusplus
}
#endif


#endif
