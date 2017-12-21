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
 * Computing some bounds of the Riemann zeta function.
 */

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
  double sl, su, snew, smid;
  int steps = 1001;        //Increase for better bounds.

  for (int i = 2; i < 7; i++) {
    sl = 0;
    smid = 0;
    su = M_PI + Eps();  //Upper bound on Pi.
    setRoundUp();
    su = pow(su,2)/6;   //Upper bound on actual value for s=2.
    setRoundDown();

    for (int k = 1; k < steps; k++) {
      snew = 1.0/pow(k,i);    //Calculating rigorous lower bound using first
      sl = sl + snew;         //1000 terms.
      snew = 1.0/pow(k,2);    //We will also need a rigorous bound for s=2
      smid = smid + snew;     //for later.
    }

    cout << "Lower bound for exponent s=" << i << " is: " << sl << endl;

    setRoundUp();
    su = su - smid;           //Removing first 1000 terms for s=2 from upper
                              //to replace with terms for s=i.
    for (int k = 1; k < steps; k++) {
      snew = 1.0/pow(k,i);    //Adding new terms for s=i to the upper bound.
      su = su + snew;
    }

    cout << "Upper bound for exponent s=" << i << " is: " << su << endl;

    setRoundNear();
  }

  return EXIT_SUCCESS;
}
