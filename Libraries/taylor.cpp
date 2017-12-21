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
#include<vector>
#include<functional>
#include"interval.hpp"
#include"taylor.hpp"

namespace taylor
{

  /*
   * Class methods
   */

  std::vector<interval::Interval> Taylor::coeff()
  {
    return this->coeff_;
  }

  interval::Interval Taylor::coeff(int N)
  {
    return this->coeff_.at(N);
  }

  int Taylor::size()
  {
    return this-> size_;
  }

  void Taylor::set_coeff(std::vector<interval::Interval> vect)
  {
    this->coeff_ = vect;
    this->size_ = vect.size();
  }

  void Taylor::set_coeff(interval::Interval x, int k)
  {
    this->coeff_.at(k) = x;
  }

  void Taylor::append(interval::Interval x)
  {
    this->coeff_.push_back(x);
    this->size_ ++;
  }

  void Taylor::resize(int k)
  {
    this->coeff_.resize(k, interval::Interval {0});
    this->size_ = k;
  }

  /*
   * Arithmetic operations
   */

  Taylor operator+(Taylor x, Taylor y)
  {
    int biggest = std::max(x.size(), y.size());
    x.resize(biggest);
    y.resize(biggest);
    Taylor result;
    for (int i = 0; i < biggest; i++) {
      result.append(x.coeff(i) + y.coeff(i));
    }

    return result;
  }

  Taylor operator-(Taylor x, Taylor y)
  {
    int biggest = std::max(x.size(), y.size());
    x.resize(biggest);
    y.resize(biggest);
    Taylor result;
    for (int i = 0; i < biggest; i++) {
      result.append(x.coeff(i) - y.coeff(i));
    }

    return result;
  }

  Taylor operator*(Taylor x, Taylor y)
  {
    int biggest = std::max(x.size(), y.size());
    x.resize(2 * biggest);
    y.resize(2 * biggest);
    Taylor result;
    interval::Interval tmp;
    for (int i = 0; i < 2 * biggest - 1; i++) {
      tmp = 0;
      for (int j = 0; j <= i; j++) {
        tmp = tmp + x.coeff(j) * y.coeff(i - j);
      }
      result.append(tmp);
    }

    return result;
  }

  Taylor operator/(Taylor x, Taylor y)
  {
    int biggest = std::max(x.size(), y.size());
    x.resize(2 * biggest);
    y.resize(2 * biggest);
    taylor::Taylor result;
    interval::Interval tmp;
    for (int i = 0; i < 2 * biggest - 1; i++) {
      tmp = 0;
      for (int j = 0; j < i; j++) {
        tmp = tmp + result.coeff(j) * y.coeff(i - j);
      }
      tmp = x.coeff(i) - tmp;
      tmp = tmp / y.coeff(0);
      result.append(tmp);
    }

    return result;
  }

  /*
   * Functions
   */

  Taylor taylor_derivative(Taylor x)
  {
    Taylor result;
    for (int i = 1; i < x.size(); i++) {
      result.append(x.coeff(i) * interval::Interval {i});
    }
    return result;
  }

  Taylor taylor_exp(Taylor x, int n)
  {
    Taylor result;
    interval::Interval tmp;
    x.resize(n+1);
    result.append(interval::interval_exp(x.coeff(0)));
    for (int i = 1; i < n+1; i++) {
      tmp = 0;
      for (int j = 1; j <= i; j++) {
        tmp = tmp + j * x.coeff(j) * result.coeff(i - j);
      }
      tmp = tmp / i;
      result.append(tmp);
    }

    return result;
  }

  //note: this computes the taylor coefficients of log(x), not log(1+x)
  Taylor taylor_log(Taylor x, int n) {
    Taylor result;
    interval::Interval tmp;
    x.resize(n+1);
    result.append(interval::interval_log(x.coeff(0)));
    for (int i = 1; i < n+1; i++) {
      tmp = 0;
      for (int j = 1; j < i; j++) {
        tmp = tmp + j * result.coeff(j) * x.coeff(i - j);
      }
      tmp = x.coeff(i) - tmp / i;
      tmp = tmp / x.coeff(0);
      result.append(tmp);
    }

    return result;
  }

  //use first element for sin(x), second element for cos(x)
  std::pair<Taylor, Taylor> taylor_sincos(Taylor x, int n)
  {
    std::pair<Taylor, Taylor> result;
    Taylor vector_sin, vector_cos;
    interval::Interval tmp_sin, tmp_cos;
    x.resize(n+1);
    vector_sin.append(interval::interval_sin(x.coeff(0)));
    vector_cos.append(interval::interval_cos(x.coeff(0)));
    for (int i = 1; i < n+1; i++) {
      tmp_sin = 0;
      tmp_cos = 0;
      for (int j = 1; j < i+1; j++) {
        tmp_sin = tmp_sin + j * x.coeff(j) * vector_cos.coeff(i - j);
        tmp_cos = tmp_cos - j * x.coeff(j) * vector_sin.coeff(i - j);
      }
      tmp_sin = tmp_sin / i;
      tmp_cos = tmp_cos / i;
      vector_sin.append(tmp_sin);
      vector_cos.append(tmp_cos);
    }
    result.first = vector_sin;
    result.second = vector_cos;

    return result;
  }

  /*
   * Operators
   */

  // Riemann term for integration of functions
  interval::Interval taylor_riemannterm(std::function<Taylor(Taylor, int)> f,
                                        interval::Interval a,
                                        int deg)
  {
    interval::Interval mid {interval::interval_mid(a)};
    interval::Interval rad {interval::interval_rad(a)};
    Taylor coeffs {mid, interval::Interval{1}};
    coeffs = f(coeffs, deg);
    interval::Interval sum {coeffs.coeff(0) * rad};
    for (int i = 2; i <= deg; i += 2) {
      sum = sum + coeffs.coeff(i) * interval::interval_pow(rad, i+1) /
        interval::Interval {i+1};
    }
    Taylor error {a, interval::Interval {1}};
    error = f(error, deg);
    interval::Interval eps {interval::interval_abs(error.coeff(deg) -
                                                   coeffs.coeff(deg))};
    eps = eps - eps;
    sum = sum + eps * interval::interval_pow(rad, deg + 1) /
      interval::Interval {deg + 1};
    return interval::Interval {2} * sum;
  }

  // Integration of functions
  interval::Interval taylor_integrate(std::function<Taylor(Taylor, int)> f,
                                      interval::Interval a,
                                      int deg,
                                      double tol)
  {
    interval::Interval quad = taylor_riemannterm(f, a, deg);
    if (interval::interval_diam(quad).upper() < tol) {
      return quad;
    }
    else {
      interval::Interval lower {a.lower(), interval::interval_mid(a).upper()};
      interval::Interval upper {interval::interval_mid(a).lower(), a.upper()};
      return taylor_integrate(f, lower, deg, tol / 2) +
        taylor_integrate(f, upper, deg, tol / 2);
    }

  }

} //namespace taylor
