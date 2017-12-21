/*
 * Copyright 2017 Dan Lilja
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Library for controlling the floating point rounding mode.
 */

#include<math.h>

#if defined(__linux)
#include<fenv.h>
#define ROUND_DOWN FE_DOWNWARD
#define ROUND_UP FE_UPWARD
#define ROUND_NEAR FE_TONEAREST
#define ROUND_ZERO FE_TOWARDZERO
#endif

#if defined(__sparc)
#include <ieeefp.h>
#define ROUND_DOWN FP_RM
#define ROUND_UP FP_RP
#define ROUND_NEAR FP_RN
#endif

void setRoundDown() {
    fesetround(ROUND_DOWN);
}
void setRoundUp() {
    fesetround(ROUND_UP);
}
void setRoundNear() {
    fesetround(ROUND_NEAR);
}
void setRoundZero() {
    fesetround(ROUND_ZERO);
}
