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

#include<cmath>
#include<iostream>
#include<functional>
#include"../Libraries/interval.hpp"
#include"../Libraries/taylor.hpp"
#include"../Libraries/taylor_double.hpp"
#include"../Libraries/autodiff.hpp"

using namespace std;
using namespace interval;
using namespace autodiff;

/*
 * It may not be optimal to use global variables but it would require quite
 * some rewriting to fix the methods without using these global variables so
 * I'll leave it like this.
 */

double g_xLo, g_xHi;
double g_TLo, g_THi;
double g_tol;
int g_deg;
int g_steps;

// Doubles

pair<double, double> taylor_double_step(double x0, double y0, int deg, double h)
{
  pair<double, double> result;
  taydbl::Taylor x, y;
  double x_tmp {0}, y_tmp {0};
  double step_size {h};

  x.append(x0);
  y.append(y0);
  for (int i = 1; i < deg+1; i++) {
    x.append(y.coeff(i-1) / i);
    y.append(0 - taylor_sincos(x, i).first.coeff(i-1) / i);
  }
  for (int i = 0; i < deg+1; i++) {
    x_tmp = x_tmp + x.coeff(i) * pow(step_size, i);
    y_tmp = y_tmp + y.coeff(i) * pow(step_size, i);
  }
  result.first = x_tmp;
  result.second = y_tmp;
  return result;
}

pair<double, double> taylor_double_method(double x0,
                                          double y0,
                                          double T,
                                          int deg,
                                          int steps)
{
  double step_size = 1. / steps;
  pair<double, double> result {x0, y0};

  for (int i = 0; i < steps * T; i++) {
    result = taylor_double_step(result.first, result.second, deg, step_size);
  }
  return result;
}

// Intervals

// Producing rigorous Taylor coefficient enclosures
pair<taylor::Taylor, taylor::Taylor> taylor_thin(Interval x0,
                                                 Interval y0,
                                                 Interval time_step,
                                                 int deg)
{
  taylor::Taylor coeffs_upper;
  taylor::Taylor coeffs_lower;
  Interval tmp;
  pair<taylor::Taylor, taylor::Taylor> result;

  coeffs_upper.append(Interval{x0.upper()});
  coeffs_lower.append(Interval{x0.lower()});
  coeffs_upper.append(Interval{y0.upper()});
  coeffs_lower.append(Interval{y0.lower()});

  for (int i = 0; i < deg - 2; i++) {
    tmp = Interval{0} - taylor::taylor_sincos(coeffs_upper, i).first.coeff(i);
    tmp = tmp / Interval{i + 2};
    tmp = tmp / Interval{i + 1};
    coeffs_upper.append(tmp);
    tmp = Interval{0} - taylor::taylor_sincos(coeffs_lower, i).first.coeff(i);
    tmp = tmp / Interval{i + 2};
    tmp = tmp / Interval{i + 1};
    coeffs_lower.append(tmp);
  }
  result.first = coeffs_upper;
  result.second = coeffs_lower;
  return result;
}

// Producing the seed for the error bound for the rigorous Taylor method. Note
// that this does not use a dynamic computation of the maximal time_step size or
// any other fancy stuff, it's just a stupid bound.
pair<Interval, Interval> wide_bound(Interval x0,
                                    Interval y0,
                                    Interval time_step)
{
  Interval x_temp, y_temp, x1, y1;
  x_temp = x0 + time_step - time_step;
  x1 = x_temp;
  y_temp = y0 - interval_sin(x_temp);
  y1 = y_temp;
  int i{0};

  while (i < 20) {
    x1 = x_temp + y_temp * time_step + interval_square(time_step) * Interval{-0.5,0.5};
    y1 = y_temp - interval_sin(x_temp) * time_step;
    x_temp = x1;
    y_temp = y1;
    ++i;
  }

  return pair<Interval, Interval> {x_temp, y_temp};
}

// Producing rigorous Taylor error enclosures
pair<Interval, Interval> taylor_wide(Interval x0,
                                     Interval y0,
                                     Interval time_step,
                                     int deg)
{
  pair<Interval, Interval> xy0 = wide_bound(x0, y0, time_step);
  Interval tmp_x = xy0.first;
  Interval tmp_y = xy0.second;
  Interval tmp;
  taylor::Taylor coeffs;
  pair<Interval, Interval> result;

  coeffs.append(tmp_x);
  coeffs.append(tmp_y);

  for (int i = 0; i < deg - 1; ++i) {
    tmp = Interval{0} - taylor::taylor_sincos(coeffs, i).first.coeff(i);
    tmp = tmp / Interval{i + 2};
    tmp = tmp / Interval{i + 1};
    coeffs.append(tmp);
  }

  result.first = coeffs.coeff(deg);
  result.second = Interval{deg} * result.first;
  return result;
}

pair<Interval, Interval> taylor_step(Interval x0,
                                     Interval y0,
                                     Interval time_step,
                                     int deg)
{
  pair<taylor::Taylor, taylor::Taylor> thin;
  pair<Interval, Interval> wide, result;
  Interval x_upper{Interval{x0.upper()}};
  Interval x_lower{Interval{x0.lower()}};
  Interval y_upper{Interval{0}};
  Interval y_lower{Interval{0}};
  thin = taylor_thin(x0, y0, time_step, deg);
  wide = taylor_wide(x0, y0, time_step, deg);

  for (int i = 0; i < deg - 1; ++i) {
    x_upper = x_upper + thin.first.coeff(i + 1) * interval_pow(time_step, i + 1);
    x_lower = x_lower + thin.second.coeff(i + 1) * interval_pow(time_step, i + 1);
    y_upper = y_upper + Interval{i + 1} * thin.first.coeff(i+1) * interval_pow(time_step, i);
    y_lower = y_lower + Interval{i + 1} * thin.second.coeff(i+1) * interval_pow(time_step, i);
  }

  result.first = interval_hull(x_upper, x_lower) + wide.first * interval_pow(time_step, deg);
  result.second = interval_hull(y_upper, y_lower) + wide.second * interval_pow(time_step, deg - 1);
  return result;
}

pair<Interval, Interval> taylor_method(Interval x0,
                                       Interval y0,
                                       Interval T,
                                       int steps,
                                       int deg)
{
  Interval adjustedT{T - Interval{T.lower()}};
  Interval lowerTimeStep{Interval{T.lower() / steps}};
  Interval upperTimeStep{Interval{adjustedT.upper() / steps}};
  Interval x1{x0};
  Interval y1{y0};
  pair<Interval, Interval> result{x0, y0};

  for(int i = 0; i < steps; ++i) {
    result = taylor_step(x1, y1, lowerTimeStep, deg);
    x1 = result.first;
    y1 = result.second;
  }


  for(int i = 0; i < steps; ++i) {
    result = taylor_step(result.first, result.second, upperTimeStep, deg);
    x1 = interval_hull(result.first, x1);
    y1 = interval_hull(result.second, y1);
  }

  result.first = x1;
  result.second = y1;
  return result;
}

Autodiff autodiffTaylor(Autodiff a)
{
  Interval x0{g_xLo, g_xHi};
  Interval y0{0};
  Interval T{a.f()};
  pair<Interval, Interval> pair = taylor_method(x0, y0, T, g_steps, g_deg);
  return Autodiff{pair.first, pair.second};
}

int main(int argc, char const *argv[])
{
  if (argc != 8) {
    cerr << "Needs seven parameters: "
         << "Lower boundary of x-interval, "
         << "upper boundary of x-interval, "
         << "lower boundary of time guess, "
         << "upper boundary of time guess, "
         << "tolerance, "
         << "degree of the Taylor method, "
         << "number of integration steps."
         << '\n' << '\n'
         << "The method for doubles uses the midpoint of the x-interval and "
         << "the midpoint of the time-interval as seeds."
         << endl;
    return EXIT_FAILURE;
  }

  g_xLo = atof(argv[1]);
  g_xHi = atof(argv[2]);
  g_TLo = atof(argv[3]);
  g_THi = atof(argv[4]);
  g_tol = atof(argv[5]);
  g_deg = atoi(argv[6]);
  g_steps = atoi(argv[7]);

  Interval i_x0 {g_xLo, g_xHi};
  Interval T {g_TLo, g_THi};

  cout << "Doing non-rigorous stuff!" << '\n' << endl;

  double x0 {interval_mid(i_x0).upper()};
  double t {interval_mid(T).upper()};
  pair<double, double> xy{x0, 0};
  while(fabs(xy.first) >= g_tol) {
    xy = taylor_double_method(x0, 0, t, g_deg, g_steps);
    t = t - ( xy.first / xy.second );
  }

  t = 4 * t;
  cout << "The non-rigorous period is: " << t << '\n' << endl;

  cout << "Now doing rigorous stuff!" << '\n'
       << "Note that the actual period is 4 times the interval below." << '\n' << endl;

  Interval i_y0{Interval{0}};
  pair<Interval, Interval> i_xy{i_x0, i_y0};

  autodiff_krawczyk(autodiffTaylor, T, g_tol);

  return EXIT_SUCCESS;
}
