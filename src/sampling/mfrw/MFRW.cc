// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file MFRW.cc
 * Implementation of the multifractal random walk interface.
 */
#include <cmath>

#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_log.h>

#include "SamplingException.hh"
#include "MFRW.hh"


namespace des
{
namespace sampling
{


// equations taken from: "Scale-free avalanches in multifractal random
// walk", by M. Bartolozzi
void MFRW::path(gsl_vector* p_vec, tGslRngSP p_rngBin, tGslRngSP p_rngNorm, double p_d0, double p_a0,
                double p_b, double p_lambda, boost::uint16_t p_Nc, boost::uint16_t p_T,
                double p_n0, double p_nmax)
{
    if (p_vec->size != p_T) {
        throw SamplingException(SamplingException::DIM_DONT_MATCH);
    }

    double a = p_a0/gsl_sf_log(p_lambda); // eq. 5
    double p = 1.0/(1.0 + gsl_pow_2(p_lambda)); // eq. 6
    double etaB = 2.0 * p - 1;
    double nt = p_n0;
    double sum = 0.0;
    boost::uint16_t n = p_Nc;

    gsl_vector *Ki = gsl_vector_calloc(p_Nc + 1);
    gsl_vector *eta = gsl_vector_calloc(p_T);

    // generate the discrete random numbers for eta
    double *props = new double[2];
    props[0] = p;
    props[1] = 1.0 - p;

    gsl_ran_discrete_t *lookup = gsl_ran_discrete_preproc(2, props);
    for (boost::uint16_t i = 0; i < p_T; ++i) {
        size_t event = gsl_ran_discrete(p_rngBin.get(), lookup);
        gsl_vector_set(eta, i, ((event == 0) ? -1 : 1));
    }

    // generate Ki values
    for (boost::uint16_t i = 0; i <= p_Nc; ++i) {
        double val = 1.0 / std::sqrt(i+1);
        gsl_vector_set(Ki, i, val);
    }

    for (boost::uint16_t t = 0; t < p_T; ++t) {
        // 2nd term of eq. 4
        double temp1 = gsl_vector_get(Ki, p_Nc);
        if ((t - p_Nc) > 0) {
            temp1 *= gsl_vector_get(eta, t - p_Nc - 1);
        } else {
            n = t;
            temp1 = 0.0;
        }

        // 3rd term of eq. 4
        if (t > 1) {
            sum = 0.0;
            for (boost::uint16_t i = 0; i < n-1; ++i) {
                sum += (gsl_vector_get(Ki, i+1) - gsl_vector_get(Ki, i))
                    * gsl_vector_get(eta, t-i);
            }
        }

        // eq. 4
        double phit = gsl_vector_get(Ki, 0) * gsl_vector_get(eta, t) - temp1 + sum;

        // eq. 3
        double dnt = gsl_vector_get(eta, t) + a * phit - p_b * etaB;

        nt += dnt;

        // bound the nt to avoid too large fluctuations
        if (nt < 0.0) {
            nt = 0.0;
        } else if (nt > p_nmax) {
            nt = p_nmax;
        }

        // eq. 2
        double dt = p_d0 * std::pow(p_lambda, nt);

        // eq. 1
        double x = gsl_ran_gaussian_ziggurat(p_rngNorm.get(), 1.0);
        gsl_vector_set(p_vec, t, dt * x);
    }

    // calculate the cumsum
    for (boost::uint16_t i = 1; i < p_T; ++i) {
        gsl_vector_set(p_vec, i, gsl_vector_get(p_vec, i-1) + gsl_vector_get(p_vec, i));
    }

    delete [] props;
    gsl_vector_free(Ki);
    gsl_vector_free(eta);
    gsl_ran_discrete_free(lookup);
}

}
}
