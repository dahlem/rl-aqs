// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <cmath>

#include <boost/cstdint.hpp>

#include "SCF.hh"
using des::statistics::SCF;

#include "StatsException.hh"
using des::statistics::StatsException;



double SCF::scf_func(
    const gsl_vector *const p_x,
    const gsl_vector *const p_y,
    const gsl_vector *const p_theta,
    const double p_exp,
    const double p_nugget) throw (StatsException)
{
    double r = 1.0;

    // check the assumptions about the vectors
    if (p_x->size != p_y->size) {
        throw StatsException(StatsException::DIM_MATCH_ERROR);
    }

    if (p_x->size != p_theta->size) {
        throw StatsException(StatsException::DIM_MATCH_ERROR);
    }

    if (!gsl_vector_ispos(p_theta)) {
        throw StatsException(StatsException::VEC_NOT_POSITIVE);
    }

    for (boost::uint32_t i = 0; i < p_x->size; ++i) {
        r *= exp(-pow(gsl_vector_get(p_y, i) - gsl_vector_get(p_x, i), p_exp)
                 / gsl_vector_get(p_theta, i));
    }

    r += p_nugget;

    return r;
}

void SCF::scf(
    const gsl_matrix *const p_X,
    gsl_matrix *const p_R,
    const gsl_vector *const p_theta,
    const double p_exp,
    const double p_nugget) throw (StatsException)
{
    // check the assumptions about the theta
    if (!gsl_vector_ispos(p_theta)) {
        throw StatsException(StatsException::VEC_NOT_POSITIVE);
    }

    // check the assumptions about the theta and the matrix X
    if (p_theta->size != p_X->size2) {
        throw StatsException(StatsException::VEC_MAT_COL_DIM_MATCH_ERROR);
    }

    // check the assumptions about the matrix R
    if (p_R->size1 != p_R->size2) {
        throw StatsException(StatsException::MAT_NOT_SQUARE);
    }

    // check the assumptions about the matrix X
    if (p_X->size1 < 2) {
        throw StatsException(StatsException::MAT_ROW_TOO_SMALL);
    }

    // check the assumptions about the matrix X and matrix R
    if (p_X->size1 != p_R->size1) {
        throw StatsException(StatsException::MAT_DIM_MATCH_ERROR);
    }

    // set the diagonal elements of the matrix R
    gsl_matrix_set_identity(p_R);

    // set the nugget effect
    for (size_t i = 0; i < p_R->size1; ++i) {
        gsl_matrix_set(p_R, i, i, gsl_matrix_get(p_R, i, i) + p_nugget);
    }

    for (size_t i = 0; i < (p_R->size1 - 1); ++i) {
        gsl_vector_const_view rowi = gsl_matrix_const_row(p_X, i);

        for (size_t j = i + 1; j < p_R->size1; ++j) {
            gsl_vector_const_view rowj = gsl_matrix_const_row(p_X, j);

            gsl_matrix_set(p_R, i, j,
                           scf_func(
                               &rowi.vector,
                               &rowj.vector,
                               p_theta,
                               p_exp,
                               0));
            gsl_matrix_set(p_R, j, i, gsl_matrix_get(p_R, i, j));
        }
    }
}
