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
