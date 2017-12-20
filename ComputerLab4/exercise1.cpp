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
