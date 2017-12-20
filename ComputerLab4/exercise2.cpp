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
