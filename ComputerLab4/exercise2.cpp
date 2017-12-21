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
#include<functional>
#include"../Libraries/interval.hpp"
#include"../Libraries/taylor.hpp"

using namespace std;
using namespace interval;
using namespace taylor;

Taylor integrand(Taylor x, int deg)
{
  Taylor tmp;

  return taylor_exp(tmp - x * x, deg);
}

// Computes erf(x) using adaptive rigorous integration of degree deg with
// tolerance tol.
Interval interval_erf(Interval x, int deg, double tol)
{
  Interval upper {0, x.upper()};
  Interval lower {0, x.lower()};
  upper = taylor_integrate(integrand, upper, deg, tol);
  lower = taylor_integrate(integrand, lower, deg, tol);

  return interval_hull(lower, upper);
}

int main(int argc, char const *argv[])
{
  if (argc != 5) {
    cerr << "Needs four parameters: "
         << "Lower boundary of interval x, "
         << "upper boundary of interval x, "
         << "degree of Taylor integration method, "
         << "tolerance."
         << endl;
  }

  double lo {atof(argv[1])};
  double hi {atof(argv[2])};
  int deg {atoi(argv[3])};
  double tol {atof(argv[4])};

  cout << interval_erf(Interval {lo, hi}, deg, tol) << endl;
  return EXIT_SUCCESS;
}
