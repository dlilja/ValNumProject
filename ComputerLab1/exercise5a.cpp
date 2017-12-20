/*
 * Computing lower and upper bounds of certain integrals. See more in the
 * comments file.
 */

#include <iostream>
#include <cmath>
#include "exercise3.h"

using namespace std;

int main(int argc, char const *argv[])
{
  int steps = 1024;
  double lower = 0;
  double upper = 0;

  setRoundDown();

  for (int i = 0; i < steps; i++) {
    lower = lower + 1.0/steps*exp(sin(pow(i*1.0/steps,2)));
  }

  cout << "Lower bound: " << lower << endl;

  setRoundUp();

  for (int i = 0; i < steps; i++) {
    upper = upper + 1.0/steps*exp(sin(pow((i+1)*1.0/steps,2)));
  }

  cout << "Upper bound: " << upper << endl;

  return EXIT_SUCCESS;
}
