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
