#ifndef TAYLOR_H
#define TAYLOR_H

#include<vector>
#include<functional>
#include"interval.hpp"

namespace taylor
{

  class Taylor
  {

  public:

    /*
     * Class constructors
     */

    Taylor() {};
    Taylor(interval::Interval x, interval::Interval dx) : coeff_({x, dx}),
                                                          size_(2) {};
    Taylor(std::vector<interval::Interval> vect) : coeff_(vect),
                                                   size_(int(vect.size())) {};
    Taylor(const Taylor& x) : coeff_(x.coeff_), size_(x.size_) {};
    ~Taylor() {};

    /*
     * Class methods
     */

    std::vector<interval::Interval> coeff();

    interval::Interval coeff(int N);

    int size();

    void set_coeff(std::vector<interval::Interval> vect);

    void set_coeff(interval::Interval x, int k);

    void append(interval::Interval x);

    void resize(int k);

  private:

    std::vector<interval::Interval> coeff_;
    int size_ {0};

  };

  /*
   * Arithmetic operations
   */

  Taylor operator+(taylor::Taylor x, taylor::Taylor y);

  Taylor operator-(taylor::Taylor x, taylor::Taylor y);

  Taylor operator*(taylor::Taylor x, taylor::Taylor y);

  Taylor operator/(taylor::Taylor x, taylor::Taylor y);

  /*
   * Functions
   */

  Taylor taylor_derivative(taylor::Taylor x);

  Taylor taylor_exp(taylor::Taylor x, int N);

  //Note: This computes the Taylor coefficients of log(x), not log(1+x)
  Taylor taylor_log(taylor::Taylor x, int N);

  //Use first element for sin(x), second element for cos(x)
  std::pair<Taylor, Taylor> taylor_sincos(Taylor x, int N);

  /*
   * Operators
   */

  // Riemann term for integration of functions
  interval::Interval taylor_riemannterm(std::function<Taylor(Taylor, int)> f,
                                        interval::Interval a,
                                        int deg);

  // Integration of functions
  interval::Interval taylor_integrate(std::function<Taylor(Taylor, int)> f,
                                      interval::Interval a,
                                      int deg,
                                      double tol);

} //namespace taylor


#endif
