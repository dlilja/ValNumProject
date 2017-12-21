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
#include<fstream>
#include<array>
#include"../Libraries/taylor_double.hpp"

using namespace std;
using namespace taydbl;

// Function performing each step of the Taylor method
pair<double, double> taylor_step(double x0,
                                 double y0,
                                 int N,
                                 double h)
{
  pair<double, double> result;
  Taylor x, y;
  double x_tmp {0}, y_tmp {0};
  double step_size {h};

  x.append(x0);
  y.append(y0);
  for (int i = 1; i < N+1; i++) {
    x.append(y.coeff(i-1) / i);
    y.append((x * x).coeff(i-1) / i
             + taylor_sincos(x, i).first.coeff(i-1) / i);
  }
  for (int i = 0; i < N+1; i++) {
    x_tmp = x_tmp + x.coeff(i) * pow(step_size, i);
    y_tmp = y_tmp + y.coeff(i) * pow(step_size, i);
  }

  result.first = x_tmp;
  result.second = y_tmp;
  return result;
}

/*
 * Using the Taylor method for integrating the ODE.
 * Variables are, in order: starting x value, starting y value, time to
 * integrate, order of taylor approximation, number of steps per time unit.
 * Lastly, plots the trajectory of the given point.
 */
pair<double, double> taylor_method(double x0,
                                   double y0,
                                   double T,
                                   int N,
                                   int steps)
{
  double step_size = 1. / steps;
  pair<double, double> result {x0, y0};

  ofstream file;
  file.open("output.csv");
  file << result.first << ", " << result.second << '\n';

  for (int i = 0; i < steps * T; i++) {
    result = taylor_step(result.first, result.second, N, step_size);
    file << result.first << ", " << result.second << '\n';
  }

  file.close();
  system("gnuplot -p -e \"plot 'output.csv' with lines\"");

  return result;
}

// Function to plot the vector field of the ODE
void plot_phase_portrait()
{
  array<double, 31> xspace, yspace, deltax, deltay;
  double shift, r;

  for (int i = 0; i < 31; i++) {
    shift = 4. * i / 30;
    xspace[i] = -2 + shift;
    yspace[i] = -2 + shift;
    deltax[i] = -2 + shift;
    deltay[i] = pow(-2 + shift, 2) + sin(-2 + shift);
  }

  ofstream file;
  file.open("output.csv");

  for (int i = 0; i < 31; i++) {
    for (int j = 0; j < 31; j++) {
      r = 4 * sqrt(1 + pow(deltax[i],2) + pow(deltay[j],2));
      file << xspace[i] << ", "
           << yspace[j] << ", "
           << deltax[j] / r << ", "
           << deltay[i] / r << endl;
    }
  }

  file.close();

  system("gnuplot -p -e \"plot 'output.csv' with vectors unset legend\"");
}

int main(int argc, char const *argv[])
{
  if (argc != 6) {
    cerr << "Needs 5 parameters: "
         << "Starting x-value, "
         << "Starting y-value, "
         << "time to integrate over, "
         << "degree of Taylor polynomials to use, "
         << "number of steps per time unit."
         << endl;
    return EXIT_FAILURE;
  }

  double x0 {atof(argv[1])};
  double y0 {atof(argv[2])};
  double T {atof(argv[3])};
  int deg {atoi(argv[4])};
  int steps {atoi(argv[5])};
  pair<double, double> integrated {taylor_method(x0, y0, T, deg, steps)};
  cout << "Integrated coordinates (x, y) are: " << "("
       << integrated.first << ", "
       << integrated.second << ")"
       << endl;

  // plot_phase_portrait(); //Uncomment this line to plot the vector field.
}
