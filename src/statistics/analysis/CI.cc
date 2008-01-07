// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <cmath>

#include <gsl/gsl_math.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_statistics.h>

#include "CI.hh"
using des::statistics::CI;



bool CI::isConfidentWithPrecision(
    const double p_data[],
    const int p_size,
    double p_alpha,
    double p_error) 
{
    double df = p_size - 1;
    double nu = 1 - (p_alpha / 2);
    double relAdjError = p_error / (1 + p_error);
    double t = gsl_cdf_tdist_Pinv(nu, df);
    double mean = gsl_stats_mean(p_data, 1, p_size);
    double sv = gsl_stats_variance_m(p_data, 1, p_size, mean);
    double ciHalfLength = 0.0;
    
    if (gsl_isnan(sv)) {
        return 1;
    }

    ciHalfLength = t * sqrt(sv / p_size);

    if ((ciHalfLength / fabs(mean)) <= relAdjError) {
        return true;
    } else {
        return false;
    }
}

