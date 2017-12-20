#include<iostream>
#include<cmath>
#include"../Libraries/interval.hpp"

using namespace std;
using namespace interval;

void addition_test()
{
  Interval a(0, 1), b(0, 1);
  Interval sum = a + b;
  cout << "[0,1] + [0,1] is:" << endl;
  cout << sum.lower() << '\n' << sum.upper() << endl;
}

void subtraction_test()
{
  Interval a(0, 1), b(0, 1);
  Interval sub = a - b;
  cout << "[0,1] - [0,1] is:" << endl;
  cout << sub.lower() << '\n' << sub.upper() << endl;
}

void mult_test()
{
  Interval a(0, 1), b(-1, 2);
  Interval mult = a * b;
  cout << "[0,1] * [-1,2] is:" << endl;
  cout << mult.lower() << '\n' << mult.upper() << endl;
}

void div_test()
{
  Interval a(0, 2), b(1, 2);
  cout << "[0,2] / [1,2] is:" << endl;
  Interval div = a / b;
  cout << div.lower() << '\n' << div.upper() << endl;

  a = Interval(0, 2);
  b = Interval(-2, 2);
  cout << "[0,2] / [-2,2] is:" << endl;
  div = a / b;
  cout << div.lower() << '\n' << div.upper() << endl;

}

void cap_test()
{
  Interval a(-1, 1), b(0, 2);
  cout << "Intersection of [-1,1] and [0,2] is:" << endl;
  Interval intersection = interval_cap(a,b);
  cout << intersection.lower() << '\n' << intersection.upper() << endl;

  a = Interval(-1, 0);
  b = Interval(1, 2);
  cout << "Intersection of [-1,0] and [1,2] is:" << endl;
  intersection = interval_cap(a,b);
  cout << intersection.lower() << '\n' << intersection.upper() << endl;
}

void hull_test()
{
    Interval a(0, 1), b(2, 3);
    cout << "Hull of [0,1] and [2,3] is:" << endl;
    Interval hull = interval_hull(a,b);
    cout << hull.lower() << '\n' << hull.upper() << endl;
}

void pow_test()
{
    Interval a(-1,2);
    Interval square = interval_pow(a, 2);
    cout << "The square of [-1,2] is:" << endl;
    cout << square.lower() << '\n' << square.upper() << endl;
    Interval three = interval_pow(a, 3);
    cout << "[-1,2] to the power 3 is:" << endl;
    cout << three.lower() << '\n' << three.upper() << endl;
    Interval negative = interval_pow(Interval(1, 2), -1);
    cout << "Inverse of [1,2] is:" << endl;
    cout << negative.lower() << '\n' << negative.upper() << endl;
}

void exp_test()
{
    cout << "The exponential of [0,1] is:" << endl;
    Interval exponential = interval_exp(Interval(0, 1));
    cout << exponential.lower() << '\n' << exponential.upper() << endl;
}

void log_test()
{
    cout << "The logarithm of [1,2] is:" << endl;
    Interval logarithm = interval_log(Interval(1, 2));
    cout << logarithm.lower() << '\n' << logarithm.upper() << endl;
    cout << "The logarithm of [-1,2] is:" << endl;
    logarithm = interval_log(Interval(-1, 2));
    cout << logarithm.lower() << '\n' << logarithm.upper() << endl;
}

void sin_test()
{
    cout << "Sine of [0,10] is:" << endl;
    Interval sine = interval_sin(Interval(0, 10));
    cout << sine.lower() << '\n' << sine.upper() << endl;
    cout << "Sine of [1,4] is:" << endl;
    sine = interval_sin(Interval(1, 4));
    cout << sine.lower() << '\n' << sine.upper() << endl;
}

void cos_test()
{
    cout << "Cosine of [0,10] is:" << endl;
    Interval cosine = interval_cos(Interval(0, 10));
    cout << cosine.lower() << '\n' << cosine.upper() << endl;
    cout << "Cosine of [1,4] is:" << endl;
    cosine = interval_cos(Interval(1, 4));
    cout << cosine.lower() << '\n' << cosine.upper() << endl;
}

void sqrt_test()
{
    cout << "Sqrt of [1,4] is:" << endl;
    Interval isqrt = interval_sqrt(Interval(1, 4));
    cout << isqrt.lower() << '\n' << isqrt.upper() << endl;
    cout << "Sqrt of [-2,2] is:" << endl;
    isqrt = interval_sqrt(Interval(-2, 2));
    cout << isqrt.lower() << '\n' << isqrt.upper() << endl;
}

int main(int argc, char const *argv[])
{
    addition_test();
    subtraction_test();
    mult_test();
    div_test();
    cap_test();
    hull_test();
    pow_test();
    exp_test();
    log_test();
    sin_test();
    cos_test();
    sqrt_test();

    return EXIT_SUCCESS;
}
