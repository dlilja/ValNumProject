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

#ifndef TAYLOR_DOUBLE_H
#define TAYLOR_DOUBLE_H

#include<vector>

namespace taydbl
{

  class Taylor
  {

  public:

    /*
     * Class constructors
     */

    Taylor() {};
    Taylor(double x, double dx) : coeff_({x, dx}), size_(2) {};
    Taylor(std::vector<double> vect) : coeff_(vect), size_(int(vect.size())) {};
    Taylor(const Taylor& x) : coeff_(x.coeff_), size_(x.size_) {};
    ~Taylor() {};

    /*
     * Class methods
     */

    std::vector<double> coeff();

    double coeff(int N);

    int size();

    void set_coeff(std::vector<double> vect);

    void set_coeff(double x, int k);

    void append(double x);

    void resize(int k);

  private:

    std::vector<double> coeff_;
    int size_ {0};

  };

  /*
   * Basic operators
   */

  Taylor operator+(taydbl::Taylor x, taydbl::Taylor y);

  Taylor operator-(taydbl::Taylor x, taydbl::Taylor y);

  Taylor operator*(taydbl::Taylor x, taydbl::Taylor y);

  Taylor operator/(taydbl::Taylor x, taydbl::Taylor y);

  /*
   * Functions
   */

  Taylor taylor_exp(taydbl::Taylor x, int N);

  Taylor taylor_log(taydbl::Taylor x, int N);

  std::pair<Taylor, Taylor> taylor_sincos(Taylor x, int N);

}

#endif
