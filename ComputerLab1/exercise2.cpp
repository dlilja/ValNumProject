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
 * The Kahan summation algorithm.
 */

#include<iostream>
#include<cmath>

int main(int argc, char const *argv[])
{
	int i;
	double s,t,c,y;
	s = 0;
	c = 0;
	for(i = 0; i < argc; ++i) {
		y = atof(argv[i]) - c;
		t = s + y;
		c = t - s;
		c = c - y;
		s = t;
	}
	c = -c;
	printf("%16.14e %16.14e \n", s, c);
	return EXIT_SUCCESS;
}
