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
