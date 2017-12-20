#include<cmath>
#include<iostream>
#include"../Libraries/interval.hpp"
#include"../Libraries/autodiff.hpp"

using namespace std;
using namespace interval;
using namespace autodiff;

Autodiff func(Autodiff x)
{
  return autodiff_sin(autodiff_pow(x,2)) + autodiff_cos(x);
}

int main(int argc, char const *argv[])
{
  if (argc != 3) {
    cerr << "Needs two parameters: The lower and upper bound on the interval"
         << " over which the range is to be computed."
         << endl;
    return EXIT_FAILURE;
  }

  double lo {atof(argv[1])};
  double hi {atof(argv[2])};
  Autodiff x {Interval {lo,hi}, Interval {1, 1}};
  Autodiff y = func(x);
  cout << "The lower boundary of f is: " << y.f().lower() << '\n'
       << "The upper boundary of f is: " << y.f().upper() << '\n'
       << "The upper boundary of df is: " << y.df().lower() << '\n'
       << "The lower boudnary of df is: " << y.df().upper() << endl;

  return EXIT_SUCCESS;
}
