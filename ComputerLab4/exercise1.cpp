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

#include<iostream>
#include<cmath>
#include"../Libraries/interval.hpp"
#include"../Libraries/autodiff.hpp"

using namespace std;
using namespace autodiff;
using namespace interval;

Autodiff funct(Autodiff a)
{
  return autodiff_sin(a)+a+autodiff_cos(a) - Autodiff{2.5};
}

int main(int argc, char const *argv[])
{
  Interval a {0.5,4.5};
  double tol {pow(2,-10)};
  autodiff_krawczyk(funct, a, tol);
  return EXIT_SUCCESS;
}
