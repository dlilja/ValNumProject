#include<iostream>
#include<vector>
#include<functional>
#include<cmath>
#include"../Libraries/interval.hpp"
#include"../Libraries/autodiff.hpp"

using namespace std;
using namespace interval;
using namespace autodiff;

Autodiff funct(Autodiff a)
{
  return autodiff_sin(a) * autodiff_cos(a) + Autodiff(1.0/2) -
    (a + Autodiff(1.0)) *
    autodiff_sin(autodiff_pow(a + Autodiff(1.0/2), 2)) +
    autodiff_exp(-a);
}

void bisect(Interval domain, double tol, int &count)
{
  Autodiff a {domain};
  a = funct(a);
  cout << domain << "  " << a.f() << "  " << a.df() <<'\n';
  if (Interval {0} <= a.f()) {
    if (Interval {0} <= a.df()) {
      bisect(Interval(domain.lower(),interval_mid(domain).upper()),
             tol,count);
      bisect(Interval(interval_mid(domain).lower(),domain.upper()),
             tol,count);
    }
    else {
      count++;
    }
  }
}

int main(int argc, char const *argv[])
{
  double tol {pow(2,-10)};
  autodiff_krawczyk(funct, Interval {0,2}, tol);
  return EXIT_SUCCESS;
}
