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

// See pdf for explanation of how I solved this one and further comments.

#include <iostream>
#include <cmath>
#include "exercise3.h"

using namespace std;

double Eps()  //Function to compute machine espilon.
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
  int steps = 1024;  //Amount of steps for each interval, increase for better
                     //accuracy.
  int stop = 128;    //The tail starts at 3*pi*stop, increase for better
                     //accuracy.
  double lowerHead, upperHead, lowerTail, upperTail;
  double lower = 0;
  double upper = 0;
  double upperPi = M_PI + Eps();
  double lowerPi = M_PI - Eps();

  //Finding the lower bound.

  setRoundDown();

  //Lower bound for the main part.

  for (int i = 0; 4*i+3 < 3*stop; i++) {
    for (int k = 0; k < steps; k++) {
      lower = lower + 1.0/steps * sin(upperPi*(1.0/2+(k+1)*1.0/steps))
        * 4*pow((4*i+1)+2*(k+1)*1.0/steps,-2);
    }
    for (int k = 0; k < steps; k++) {
      lower = lower + 1.0/steps * sin(lowerPi*(k*1.0/steps - 1.0/2))
        * 4*pow((4*i+3)+2*(k+1)*1.0/steps,-2);
    }
  }

  //Lower bound for the head.

  lowerHead = 0;

  for (int k = 0; k < steps; k++) {
    lowerHead = lowerHead + 1.0/steps*(upperPi/2-1)
      * sin(1+(lowerPi/2-1)*k/steps)
      * pow(1+(upperPi/2-1)*(k+1)*1.0/steps,-2);
  }

  lowerHead = lowerHead/2;

  //Lower bound for the tail.

  lowerTail = 1.0/(2*upperPi*3*stop);

  lower = lower/(2*upperPi) + lowerHead - lowerTail;

  cout << "Lower bound is: " << lower << endl;

  //Finding the upper bound.

  setRoundUp();

  //Upper bound for the main part.

  for (int i = 0; 4*i+5 < 3*stop; i++) {
    for (int k = 0; k < steps; k++) {
      upper = upper + 1.0/steps * sin(lowerPi*(1.0/2+k*1.0/steps))
        * 4*pow((4*i+1)+2*k*1.0/steps,-2);
    }
    for (int k = 0; k < steps; k++) {
      upper = upper + 1.0/steps * sin(upperPi*((k+1)*1.0/steps - 1.0/2))
        * 4*pow((4*i+3)+2*k*1.0/steps,-2);
    }
  }

  //Upper bound for the head.

  upperHead = 0;

  for (int k = 0; k < steps; k++) {
    upperHead = upperHead + 1.0/steps*(lowerPi/2-1)
      * sin(1+(upperPi/2-1)*(k+1)/steps)
      * pow(1+(lowerPi/2-1)*k*1.0/steps,-2);
  }

  upperHead = upperHead/2;

  //Upper bound for the tail.

  upperTail = 1.0/(2*lowerPi*3*stop);

  upper = upper/(2*lowerPi) + upperHead + upperTail;

  cout << "Upper bound is: " << upper << endl;

  setRoundNear();

  return EXIT_SUCCESS;
}
