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
#include"../Libraries/autodiff.hpp"
#include"../Libraries/taylor.hpp"

using namespace std;
using namespace interval;
using namespace autodiff;
using namespace taylor;

Taylor integrand(Taylor x, int deg)
{
  Taylor tmp {Interval {0}, Interval {0}};
  return taylor_exp(tmp - x * x / Taylor { Interval {2}, Interval {0} }, deg);
}

Interval interval_integrand(Interval x)
{
  Interval tmp {0, 0};
  return interval_exp(tmp - interval_square(x) / Interval {2});
}

// Computes corrected erf(x) - 0.95 using adaptive rigorous integration of
// degree deg with tolerance tol.
Autodiff autodiff_erf(Autodiff x)
{
  int deg {25};
  double tol {0.000001};
  Interval correction { Interval {2} / Interval {M_PI} };
  correction = interval_sqrt(correction);
  Taylor taylor_x {x.f(), x.df()};
  Interval upper {0, x.f().upper()};
  Interval lower {0, x.f().lower()};

  upper = taylor_integrate(integrand, upper, deg, tol);
  lower = taylor_integrate(integrand, lower, deg, tol);

  return Autodiff {correction * interval_hull(lower, upper) - Interval {0.95},
      correction * integrand(taylor_x, deg).coeff(0)};
}

// Computes corrected erf(x) - 0.99 using adaptive rigorous integration of
// degree deg with tolerance tol.
Autodiff autodiff_erf_99(Autodiff x)
{
  int deg {20};
  double tol {0.000001};
  Interval correction { Interval {2} / Interval {M_PI} };
  correction = interval_sqrt(correction);
  Taylor taylor_x {x.f(), x.df()};
  Interval upper {0, x.f().upper()};
  Interval lower {0, x.f().lower()};

  upper = taylor_integrate(integrand, upper, deg, tol);
  lower = taylor_integrate(integrand, lower, deg, tol);

  return Autodiff {correction * interval_hull(lower, upper) - Interval {0.99},
      correction * integrand(taylor_x, deg).coeff(0)};
}

int main(int argc, char const *argv[])
{
  Interval domain {1, 4};

  cout << "The following zeros are the corresponding a and b respectively"
       << endl;

  cout << "For 0.95: " << '\n';
  autodiff_krawczyk(autodiff_erf, domain, 0.01);

  cout << "For 0.99: " << '\n';
  autodiff_krawczyk(autodiff_erf_99, domain, 0.01);

  return EXIT_SUCCESS;
}
