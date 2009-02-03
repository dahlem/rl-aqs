// Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file CI.cc
 * Implementation of the declarations in the header file CI.hh.
 */
#include <cmath>

#include <gsl/gsl_math.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_statistics.h>

#include "CI.hh"
#include "Stats.hh"

namespace des
{
namespace statistics
{



bool CI::isConfidentWithPrecision(
    const double p_data[],
    const int p_size,
    double p_alpha,
    double p_error)
{
    double mean = gsl_stats_mean(p_data, 1, p_size);
    double sv = gsl_stats_variance_m(p_data, 1, p_size, mean);

    return CI::isConfidentWithPrecision(mean, sv, p_size, p_alpha, p_error);
}


bool CI::isConfidentWithPrecision(
    const double p_xbar, const double p_sd, const double p_x,
    const int p_size,
    double p_alpha,
    double p_error)
{
    double mean = Stats::mean(p_size, p_xbar, p_x);
    double sv = Stats::variance(p_xbar, mean, p_sd, p_x) / (p_size - 1);

    return CI::isConfidentWithPrecision(mean, sv, p_size, p_alpha, p_error);
}


bool CI::isConfidentWithPrecision(
    const double p_mean, const double p_sv,
    const int p_size, double p_alpha, double p_error)
{
    // we need at least two samples
    if (p_size < 2) {
        return false;
    }

    double df = p_size - 1;
    double nu = 1 - (p_alpha / 2);
    double relAdjError = p_error / (1 + p_error);
    double t = gsl_cdf_tdist_Pinv(nu, df);
    double ciHalfLength = 0.0;

    if (gsl_isnan(p_sv)) {
        return true;
    }

    ciHalfLength = t * sqrt(p_sv / p_size);

    if ((ciHalfLength / fabs(p_mean)) <= relAdjError) {
        return true;
    } else {
        return false;
    }
}


}
}
