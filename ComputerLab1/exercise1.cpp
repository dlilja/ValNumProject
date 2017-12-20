/*
 * Computing the machine epsilon.
 */


#include<iostream>

using namespace std;

double eps()
{
	double current = 1.0;
	double last = 1.0;
	while(1+current > 1) {
    last = current;
    current = current/2;
  }

	return last;
}

int main(int argc, char const *argv[])
{
  cout << "The machine epsilon is: " << eps() << endl;
	return EXIT_SUCCESS;
}
