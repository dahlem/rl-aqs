// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

