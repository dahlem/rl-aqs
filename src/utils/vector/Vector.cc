// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Vector.cc
 * Implementation of the declarations in @ref{Vector.hh}
 */
#include <cmath>

#include "Vector.hh"

namespace des
{
namespace utils
{


double Vector::dotproduct(int p_n, DoubleSA p_vec1, DoubleSA p_vec2)
{
    int k, m, i = 0;
    double sum = 0.0;

    k = p_n / 4;
    m = p_n % 4;

    for (i = 0; i < k; i += 4) {
        sum += p_vec1[i] * p_vec2[i];
        sum += p_vec1[i + 1] * p_vec2[i + 1];
        sum += p_vec1[i + 2] * p_vec2[i + 2];
        sum += p_vec1[i + 3] * p_vec2[i + 3];
    }

    switch (m) {
      case 3:
          sum += p_vec1[i] * p_vec2[i];
          ++i;
      case 2:
          sum += p_vec1[i] * p_vec2[i];
          ++i;
      case 1:
          sum += p_vec1[i] * p_vec2[i];
    }

    return sum;
}


void Vector::mult(int p_n, DoubleSA p_vec, double p_scalar)
{
    int k, m, i = 0;

    k = p_n / 4;
    m = p_n % 4;

    for (i = 0; i < k; i += 4) {
        p_vec[i] = p_vec[i] * p_scalar;
        p_vec[i + 1] = p_vec[i + 1] * p_scalar;
        p_vec[i + 2] = p_vec[i + 2] * p_scalar;
        p_vec[i + 3] = p_vec[i + 3] * p_scalar;
    }

    switch (m) {
      case 3:
          p_vec[i] = p_vec[i] * p_scalar;
          ++i;
      case 2:
          p_vec[i] = p_vec[i] * p_scalar;
          ++i;
      case 1:
          p_vec[i] = p_vec[i] * p_scalar;
    }
}


void Vector::add(int p_n, DoubleSA p_vec1, DoubleSA p_vec2)
{
    for (int i = 0; i < p_n; ++i) {
        p_vec1[i] += p_vec2[i];
    }
}


void Vector::normalise(int p_n, DoubleSA p_vec)
{
    double length = 0.0;

    for (int i = 0; i < p_n; ++i) {
        length += p_vec[i] * p_vec[i];
    }

    length = std::sqrt(length);

    if (length > 0.0) {
        for (int i = 0; i < p_n; ++i) {
            p_vec[i] /= length;
        }
    }
}


void Vector::scale(int p_n, DoubleSA p_vec, double p_min, double p_max)
{
    for (int i = 0; i < p_n; ++i) {
        p_vec[i] = p_min + (p_max - p_min) * p_vec[i];
    }
}

}
}
