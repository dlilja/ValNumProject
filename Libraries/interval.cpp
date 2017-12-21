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
#include"interval.hpp"
#include"rounding.h"

namespace interval
{

  /*
   * Constructors
   */

  Interval::Interval() {};

  Interval::Interval(double val)
  {
    set_round_down(); //These roundings really have no effect but ideally you would want them.
    lower_ = val;
    set_round_up();
    upper_ = val;
    set_round_near();
  }

  Interval::Interval(int val)
  {
    set_round_down();
    lower_ = (double) val;
    set_round_up();
    upper_ = (double) val;
    set_round_near();
  }

  Interval::Interval(double lower, double upper)
  {
    if (upper < lower) {
      std::cout << "Error: Lower bound larger than upper bound." << std::endl;
      lower_ = NAN;
      upper_ = NAN;
    }
    else {
      set_round_down();
      lower_ = lower;
      set_round_up();
      upper_ = upper;
      set_round_near();
    }

  }

  Interval::Interval(const Interval &i)
  {
    lower_ = i.lower_;
    upper_ = i.upper_;
  }

  /*
   * Class methods
   */

  double Interval::lower()
  {
    return this->lower_;
  }

  double Interval::upper()
  {
    return this->upper_;
  }

  void Interval::set_lower(double lower)
  {
    this->lower_ = lower;
  }

  void Interval::set_upper(double upper)
  {
    this->upper_ = upper;
  }

  std::ostream &operator<<(std::ostream &os, const Interval &a)
  {
    return os << '[' << a.lower_ << ", " << a.upper_ << ']';
  }

  /*
   * Basic operators
   */

  Interval operator+(Interval a, Interval b)
  {
    set_round_down();
    double lower {a.lower() + b.lower()};
    set_round_up();
    double upper {a.upper() + b.upper()};

    return Interval {lower, upper};
  }

  Interval operator-(Interval a, Interval b)
  {
    set_round_down();
    double lower {a.lower() - b.upper()};
    set_round_up();
    double upper {a.upper() - b.lower()};
    set_round_near();

    return Interval {lower, upper};
  }

  Interval operator*(Interval a, Interval b)
  {
    double m1, m2, m3, m4, m;

    set_round_down();
    m1 = a.lower() * b.lower();
    m2 = a.lower() * b.upper();
    m3 = a.upper() * b.lower();
    m4 = a.upper() * b.upper();
    m = fmin(m1,m2);
    m = fmin(m,m3);
    m = fmin(m,m4);
    double lower {m};

    set_round_up();
    m1 = a.lower() * b.lower();
    m2 = a.lower() * b.upper();
    m3 = a.upper() * b.lower();
    m4 = a.upper() * b.upper();
    m = fmax(m1,m2);
    m = fmax(m,m3);
    m = fmax(m,m4);
    double upper {m};

    set_round_near();

    return Interval {lower, upper};
  }

  Interval operator/(Interval a, Interval b)
  {
    Interval q;

    if (b.lower() <= 0 && b.upper() >= 0) {
      std::cout << "Error: Denominator contains 0." << std::endl;
      q = Interval {NAN, NAN};
    }
    else {
      set_round_down();
      double lower {1.0 / b.upper()};
      set_round_up();
      double upper {1.0 / b.lower()};
      Interval tmp {lower, upper};
      q = Interval {a * tmp};
    }

    return q;
  }

  bool operator<=(Interval a, Interval b)
  {
    return (b.lower() <= a.lower()) && (a.upper() <= b.upper());
  }

  bool operator<(Interval a, Interval b)
  {
    return (b.lower() < a.lower()) && (a.upper() < b.upper());
  }

  Interval interval_cap(Interval a, Interval b)
  {
    Interval cap;

    if (a.upper() < b.lower() || b.upper() < a.lower()) {
      cap.set_lower(NAN);
      cap.set_upper(NAN);
    }
    else {
      cap = Interval(fmax(a.lower(),b.lower()),
                     fmin(a.upper(),b.upper()));
    }

    return cap;
  }

  Interval interval_hull(Interval a, Interval b)
  {
    return Interval {fmin(a.lower(),b.lower()),fmax(a.upper(),b.upper())};
  }

  /*
   * Functions
   * ---------
   * Many of these are not entirely rigorous since they use functions
   * from the standard library as a shortcut, e.g. sin and cos.
   */

  Interval interval_mid(Interval a)
  {
    return Interval {(a.lower() + a.upper())/2};
  }

  Interval interval_diam(Interval a)
  {
    set_round_up();
    Interval diam {a.upper() - a.lower()};
    set_round_near();
    return diam;
  }

  Interval interval_rad(Interval a)
  {
    return {interval_diam(a) / Interval {2}};
  }

  Interval interval_mag(Interval a)
  {
    set_round_up();
    return Interval {fmax(fabs(a.lower()),fabs(a.upper()))};
  }

  //Absolute value of an interval
  Interval interval_abs(Interval a)
  {
    return interval_hull(Interval {0}, interval_mag(a));
  }

  //Squaring an interval.
  Interval interval_square(Interval a)
  {
    Interval square;

    //Checking if the interval contains zero.
    if (a.lower() <= 0 && a.upper() >= 0) {
      square.set_lower(0);
      set_round_up();
      square.set_upper(fmax(pow(a.lower(),2),pow(a.upper(),2)));
    }
    else {
      set_round_down();
    	square.set_lower(fmin(pow(a.lower(),2),pow(a.upper(),2)));
    	set_round_up();
    	square.set_upper(fmax(pow(a.lower(),2),pow(a.upper(),2)));
    }

    return square;
  }

  //Taking all integer powers
  Interval interval_pow(Interval a, int n)
  {
    // If n == 0 we want to return [1,1]
    Interval power{1, 1};

    if (n == 1) {
      power = a;
    }

    if (n > 1) {
      power = interval_square(a) * interval_pow(a, n-2);
    }

    if (n < 0) {
      power = Interval{1} / interval_pow(a, -n);
    }

    return power;
  }

  Interval interval_exp(Interval a)
  {
    set_round_down();
    long double lower {expl(a.lower())};
    set_round_up();
    long double upper {expl(a.upper())};
    set_round_near();

    return Interval {(double) lower, (double) upper};
  }

  Interval interval_log(Interval a)
  {
    Interval logarithm;
    if (a.lower() <= 0) {
      std::cout << "Error: Interval contains nonpositive numbers." <<
        std::endl;
      logarithm.set_lower(NAN);
      logarithm.set_upper(NAN);
    }
    else {
      set_round_down();
      logarithm.set_lower(log(a.lower()));
      set_round_up();
      logarithm.set_upper(log(a.upper()));
      set_round_near();
    }

    return logarithm;
  }

  Interval interval_sin(Interval a)
  {
    Interval sine;

    //Big enough intervals always give the same answer.
    if (a.upper() - a.lower() > 2*M_PI) {
      sine.set_lower(-1);
      sine.set_upper(1);
    }
    else {
      //Shifting to a standard interval.
      while (a.lower() < 0) {
        a = a + Interval(2*M_PI);
      }

      //Shifting to a standard interval.
      while (a.lower() > 2*M_PI) {
        a = a - Interval(2*M_PI);
      }

      //Intervals of monotonicity.
      Interval b1 = Interval(-M_PI/2,M_PI/2);
      Interval b2 = Interval(M_PI/2,3*M_PI/2);
      Interval b3 = Interval(3*M_PI/2,5*M_PI/2);
      Interval b4 = Interval(5*M_PI/2,7*M_PI/2);
      Interval b5 = Interval(7*M_PI/2,9*M_PI/2);

      //Monotone parts of the input.
      Interval a1 = interval_cap(a,b1);
      Interval a2 = interval_cap(a,b2);
      Interval a3 = interval_cap(a,b3);
      Interval a4 = interval_cap(a,b4);
      Interval a5 = interval_cap(a,b5);

      //Lowest values on each monotone interval
      set_round_down();
      b1.set_lower(sin(a1.lower()));
      b2.set_lower(sin(a2.upper()));
      b3.set_lower(sin(a3.lower()));
      b4.set_lower(sin(a4.upper()));
      b5.set_lower(sin(a5.lower()));

      //Highest values on each monotone interval
      set_round_up();
      b1.set_upper(sin(a1.upper()));
      b2.set_upper(sin(a2.lower()));
      b3.set_upper(sin(a3.upper()));
      b4.set_upper(sin(a4.lower()));
      b5.set_upper(sin(a5.upper()));

      set_round_near();

      //Joining each monotone image
      sine = interval_hull(b1,b2);
      sine = interval_hull(sine,b3);
      sine = interval_hull(sine,b4);
      sine = interval_hull(sine,b5);
    }

    return sine;
  }

  Interval interval_cos(Interval a)
  {
    Interval cosine;

    if (a.upper() - a.lower() > 2*M_PI) {
      cosine.set_lower(-1);
      cosine.set_upper(1);
    }
    else {
      while (a.lower() < 0) {
        a = a + Interval(2*M_PI);
      }

      while (a.lower() > 2*M_PI) {
        a = a - Interval(2*M_PI);
      }

      //Intervals of monotonicity.
      Interval b1 = Interval(0,M_PI);
      Interval b2 = Interval(M_PI,2*M_PI);
      Interval b3 = Interval(2*M_PI,3*M_PI);
      Interval b4 = Interval(3*M_PI,4*M_PI);

      Interval a1 = interval_cap(a,b1);
      Interval a2 = interval_cap(a,b2);
      Interval a3 = interval_cap(a,b3);
      Interval a4 = interval_cap(a,b4);

      set_round_down();
      b1.set_lower(cos(a1.upper()));
      b2.set_lower(cos(a2.lower()));
      b3.set_lower(cos(a3.upper()));
      b4.set_lower(cos(a4.lower()));

      set_round_up();
      b1.set_upper(cos(a1.lower()));
      b2.set_upper(cos(a2.upper()));
      b3.set_upper(cos(a3.lower()));
      b4.set_upper(cos(a4.upper()));

      set_round_near();

      cosine = interval_hull(b1,b2);
      cosine = interval_hull(cosine,b3);
      cosine = interval_hull(cosine,b4);
    }

    return cosine;
  }

  /*
   * Advanced operators
   */

  //Integrating functions naively
  Interval interval_sqrt(Interval a)
  {
    Interval sqroot;

    if (a.lower() < 0) {
      sqroot = Interval(NAN);
      std::cout << "Error: Interval contains negative elements." <<
        std::endl;
    }
    else {
      set_round_down();
      sqroot.set_lower(sqrt(a.lower()));
      set_round_up();
      sqroot.set_upper(sqrt(a.upper()));
      set_round_near();
    }

    return sqroot;
  }

  //Adaptively integrating functions naively
  Interval interval_integrate(std::function<Interval(Interval)> f,
                              Interval a,
                              double tol)
  {
    if (interval_diam(f(a) * interval_diam(a)) < Interval {-tol, tol}) {
      return f(a) * interval_diam(a);
    }
    else {
      Interval lower {a.lower(), interval_mid(a).upper()};
      Interval upper {interval_mid(a).lower(), a.upper()};
      return interval_integrate(f, lower, tol) +
        interval_integrate(f, upper, tol);
    }
  }

}
