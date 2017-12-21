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
