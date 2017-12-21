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

#ifndef AUTODIFF_H
#define AUTODIFF_H

#include<functional>
#include"interval.hpp"

namespace autodiff
{

  class Autodiff
  {

  private:

    interval::Interval f_, df_;

  public:

    /*
     * Class constructors
     */

    Autodiff() : f_(interval::Interval {0}), df_(interval::Interval {0}) {};
    Autodiff(double a) : f_(interval::Interval(a)),
                         df_(interval::Interval {0}) {};
    Autodiff(interval::Interval a) : f_(a),
                                     df_(interval::Interval {1}) {};
    Autodiff(interval::Interval a, interval::Interval b) : f_(a), df_(b) {};
    //Autodiff(const Autodiff& a) : f_(a.f_), df_(a.df_) {};
    ~Autodiff() {};

    /*
     * Class methods
     */

    interval::Interval f();

    interval::Interval df();

    void set_f(interval::Interval a);

    void set_df(interval::Interval a);

  };

  /*
   * Basic operators
   */

  Autodiff operator+(Autodiff a, Autodiff b);

  Autodiff operator*(Autodiff a, Autodiff b);

  Autodiff operator-(Autodiff a, Autodiff b);

  Autodiff operator-(Autodiff a);

  Autodiff operator/(Autodiff a, Autodiff b);

  /*
   * Functions
   */

  Autodiff autodiff_pow(Autodiff a, int n);

  Autodiff autodiff_exp(Autodiff a);

  Autodiff autodiff_log(Autodiff a);

  Autodiff autodiff_sin(Autodiff a);

  Autodiff autodiff_cos(Autodiff a);

  Autodiff autodiff_sqrt(Autodiff a);

  /*
   * Advanced operators
   */

  void autodiff_krawczyk(std::function<Autodiff(Autodiff)> f,
                         interval::Interval a, double tol);

}

#endif
