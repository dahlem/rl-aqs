// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Stats.cc
 * Implementation of the declaration in the header file Stats.hh
 */
#include <cmath>

#include <gsl/gsl_math.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_statistics.h>

#include "Stats.hh"
using des::statistics::Stats;


double Stats::mean(int p_n, double p_xbar, double p_x)
{
    double d;

    d = p_x - p_xbar;

    return p_xbar + d / p_n;
}


double Stats::ssd(int p_n, double p_xbar, double p_sd, double p_x)
{
    double d;

    d = p_x - p_xbar;

    return (p_sd + d * d * (p_n - 1) / p_n);
}


bool Stats::isInPos(const double p_dat, const double p_mean, const double p_svar,
                    const int p_size, const double p_alpha)
{
    if (p_size < 2) {
        return false;
    }
    if (p_svar <= 0) {
        return false;
    }

    double df = p_size - 1;
    double nu = 1 - (p_alpha / 2);
    double t = gsl_cdf_tdist_Pinv(nu, df);
    double ciHalfLength = 0.0;

    ciHalfLength = t * sqrt(p_svar / p_size);

    if ((p_dat >= p_mean) && (p_dat < (p_mean + ciHalfLength))) {
        return true;
    } else {
        return false;
    }

}


bool Stats::isInNeg(const double p_dat, const double p_mean, const double p_svar,
                    const int p_size, const double p_alpha)
{
    if (p_size < 2) {
        return false;
    }
    if (p_svar <= 0) {
        return false;
    }

    double df = p_size - 1;
    double nu = 1 - (p_alpha / 2);
    double t = gsl_cdf_tdist_Pinv(nu, df);
    double ciHalfLength = 0.0;

    ciHalfLength = t * sqrt(p_svar / p_size);

    if ((p_dat <= p_mean) && (p_dat > (p_mean - ciHalfLength))) {
        return true;
    } else {
        return false;
    }

}
