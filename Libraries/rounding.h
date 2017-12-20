#include<fenv.h>
#define ROUND_DOWN FE_DOWNWARD
#define ROUND_UP FE_UPWARD
#define ROUND_NEAR FE_TONEAREST
#define ROUND_ZERO FE_TOWARDZERO
#define setRound fesetround

void set_round_down() {
	setRound(ROUND_DOWN);
}
void set_round_up() {
	setRound(ROUND_UP);
}
void set_round_near() {
	setRound(ROUND_NEAR);
}
void set_round_zero() {
	setRound(ROUND_ZERO);
}
