#include<iostream>
#include<functional>
#include<cmath>
#include"interval.hpp"
#include"autodiff.hpp"

using namespace interval;

namespace autodiff
{

  /*
   * Class methods
   */

  Interval Autodiff::f()
  {
    return this->f_;
  }

  Interval Autodiff::df()
  {
    return this->df_;
  }

  void Autodiff::set_f(Interval a)
  {
    this->f_ = a;
  }

  void Autodiff::set_df(Interval a)
  {
    this->df_ = a;
  }

  /*
   * Basic operators
   */

  Autodiff operator+(Autodiff a, Autodiff b)
  {
    return Autodiff(a.f() + b.f(), a.df() + b.df());
  }

  Autodiff operator*(Autodiff a, Autodiff b)
  {
    return Autodiff(a.f() * b.f(), (a.df() * b.f()) + (a.f() * b.df()));
  }

  Autodiff operator-(Autodiff a, Autodiff b)
  {
    return Autodiff(a.f() - b.f(), a.df() - b.df());
  }

  Autodiff operator-(Autodiff a)
  {
    return Autodiff(-1) * a;
  }

  Autodiff operator/(Autodiff a, Autodiff b)
  {
    return Autodiff(a.f() / b.f(), (a.df() - (a.f() / b.f()) * b.df()) / b.f());
  }

  /*
   * Functions
   */

  Autodiff autodiff_pow(Autodiff a, int n)
  {
    return Autodiff(interval_pow(a.f(), n),
                    Interval(n) *
                    interval_pow(a.f(), n-1) * a.df());
  }

  Autodiff autodiff_exp(Autodiff a)
  {
    return Autodiff(interval_exp(a.f()),
                    a.df() * interval_exp(a.f()));
  }

  Autodiff autodiff_log(Autodiff a)
  {
    return Autodiff(interval_log(a.f()), a.df() / a.f());
  }

  Autodiff autodiff_sin(Autodiff a)
  {
    return Autodiff(interval_sin(a.f()),
                    a.df() * interval_cos(a.f()));
  }

  Autodiff autodiff_cos(Autodiff a)
  {
    return Autodiff(interval_cos(a.f()),
                    Interval(-1,-1) * a.df() *
                    interval_sin(a.f()));
  }

  Autodiff autodiff_sqrt(Autodiff a)
  {
    return Autodiff(interval_sqrt(a.f()),
                    Interval(-1./2,-1./2) * a.df() /
                    interval_sqrt(a.f()));
  }

  /*
   * Advanced operators
   */

  void autodiff_krawczyk(std::function<Autodiff(Autodiff)> f,
                         Interval a, double tol)
  {
    Interval mid {interval_mid(a)};
    Autodiff autoMid {mid};
    autoMid = f(autoMid);
    Autodiff eval {f(Autodiff {a})};
    Interval ka {mid - autoMid.f()/autoMid.df() +
        (Interval {1} - eval.df()/autoMid.df()) * (a - mid) };
    Interval kap {interval_cap(ka,a)};
    if (!std::isnan(kap.lower())) {
      if (interval_diam(ka).upper() < tol) {
        if (ka < a) {
          std::cout << "Unique zero in " << ka << std::endl;
        }
        else {
          std::cout << "Possible zero in " << ka << std::endl;
        }
      }
      else {
        Interval lower {a.lower(),
            interval_mid(a).upper()};
        Interval upper {interval_mid(a).lower(),
            a.upper()};
        autodiff_krawczyk(f,lower,tol);
        autodiff_krawczyk(f,upper,tol);
      }
    }
  }

}
