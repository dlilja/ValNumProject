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

#ifndef INTERVAL_H
#define INTERVAL_H

#include<functional>

namespace interval
{

  class Interval
  {

  public:

    /*
     * Constructors
     */

    Interval();
    Interval(double val);
    Interval(int val);
    Interval(double lower, double upper);
    Interval(const Interval &i);
    ~Interval() {};

    /*
     * Class methods
     */

    double lower();

    double upper();

    void set_lower(double lower);

    void set_upper(double upper);

    friend std::ostream &operator<<(std::ostream &os, const Interval &a);

  private:

    double lower_;
    double upper_;

  };

  /*
   * Basic operators
   */

  Interval operator+(Interval a, Interval b);

  Interval operator-(Interval a, Interval b);

  Interval operator*(Interval a, Interval b);

  Interval operator/(Interval a, Interval b);

  bool operator<=(Interval a, Interval b);

  bool operator<(Interval a, Interval b);

  Interval interval_cap(Interval a, Interval b);

  Interval interval_hull(Interval a, Interval b);

  /*
   * Functions
   */

  Interval interval_mid(Interval a);

  Interval interval_diam(Interval a);

  Interval interval_rad(Interval a);

  Interval interval_mag(Interval a);

  //Absolute value of an interval
  Interval interval_abs(Interval a);

  //Squaring an interval.
  Interval interval_square(Interval a);

  //Taking non-negative powers.
  Interval interval_ppow(Interval a, int n);

  //Taking all integer powers
  Interval interval_pow(Interval a, int n);

  Interval interval_exp(Interval a);

  Interval interval_log(Interval a);

  Interval interval_sin(Interval a);

  Interval interval_cos(Interval a);

  Interval interval_sqrt(Interval a);

  /*
   * Advanced operators
   */

  //Integrating functions naively
  Interval interval_integrate(std::function<Interval(Interval)> f,
                              Interval a,
                              double tol);

}

#endif
