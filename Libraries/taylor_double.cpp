#include<cmath>
#include<vector>
#include"taylor_double.hpp"

namespace taydbl
{

  /*
   * Class methods
   */

  std::vector<double> Taylor::coeff() {
    return this->coeff_;
  }

  double Taylor::coeff(int N) {
    return this->coeff_.at(N);
  }

  int Taylor::size() {
    return this-> size_;
  }

  void Taylor::set_coeff(std::vector<double> vect) {
    this->coeff_ = vect;
    this->size_ = vect.size();
  }

  void Taylor::set_coeff(double x, int k) {
    this->coeff_.at(k) = x;
  }

  void Taylor::append(double x) {
    this->coeff_.push_back(x);
    this->size_ ++;
  }

  void Taylor::resize(int k) {
    this->coeff_.resize(k);
    this->size_ = k;
  }

  /*
   * Basic operators
   */

  Taylor operator+(Taylor x, Taylor y) {
    int biggest = std::max(x.size(), y.size());
    x.resize(biggest);
    y.resize(biggest);
    Taylor result;
    for (int i = 0; i < biggest; i++) {
      result.append(x.coeff(i) + y.coeff(i));
    }

    return result;
  }

  Taylor operator-(Taylor x, Taylor y) {
    int biggest = std::max(x.size(), y.size());
    x.resize(biggest);
    y.resize(biggest);
    Taylor result;
    for (int i = 0; i < biggest; i++) {
      result.append(x.coeff(i) - y.coeff(i));
    }

    return result;
  }

  Taylor operator*(Taylor x, Taylor y) {
    int biggest = std::max(x.size(), y.size());
    x.resize(2 * biggest);
    y.resize(2 * biggest);
    Taylor result;
    double tmp;
    for (int i = 0; i < 2 * biggest - 1; i++) {
      tmp = 0;
      for (int j = 0; j <= i; j++) {
        tmp = tmp + x.coeff(j) * y.coeff(i - j);
      }
      result.append(tmp);
    }

    return result;
  }

  Taylor operator/(Taylor x, Taylor y) {
    int biggest = std::max(x.size(), y.size());
    x.resize(2 * biggest);
    y.resize(2 * biggest);
    Taylor result;
    double tmp;
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

  Taylor taylor_exp(Taylor x, int N) {
    Taylor result;
    double tmp;
    x.resize(N+1);
    result.append(std::exp(x.coeff(0)));
    for (int i = 1; i < N+1; i++) {
      tmp = 0;
      for (int j = 1; j <= i; j++) {
        tmp = tmp + j * x.coeff(j) * result.coeff(i - j);
      }
      tmp = tmp / i;
      result.append(tmp);
    }

    return result;
  }

  Taylor taylor_log(Taylor x, int N) {
    Taylor result;
    double tmp;
    x.resize(N+1);
    result.append(std::log(x.coeff(0)));
    for (int i = 1; i < N+1; i++) {
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

  std::pair<Taylor, Taylor> taylor_sincos(Taylor x, int N)
  {
    std::pair<Taylor, Taylor> result;
    Taylor vector_sin, vector_cos;
    double tmp_sin, tmp_cos;
    x.resize(N+1);
    vector_sin.append(std::sin(x.coeff(0)));
    vector_cos.append(std::cos(x.coeff(0)));
    for (int i = 1; i < N+1; i++) {
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

}
