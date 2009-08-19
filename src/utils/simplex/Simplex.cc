// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spesimplexal exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Simplex.cc
 * Implementation of the declarations in @ref{Simplex.hh}
 */
#include "Simplex.hh"

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include <gsl/gsl_randist.h>


namespace des 
{
namespace utils 
{


void Simplex::projectionDuchi(
    int p_n, DoubleSA p_vec, double p_z, dsample::tGslRngSP p_uniform_rng)
{
    double s = 0.0;
    double rho = 0.0;
    double delta_s, delta_rho, theta;
    std::vector<boost::uint16_t> U, G, L;

    // add the set of all indeces to p_vec to the U vector
    for (boost::uint16_t i = 0; i < p_n; ++i) {
        U.push_back(i);
    }

    while (U.size() > 0) {
        boost::uint32_t k = gsl_rng_uniform_int(p_uniform_rng.get(), U.size());
        std::remove_copy_if(
            U.begin(), U.end(), std::back_inserter(G),
            std::bind2nd(std::less<boost::uint16_t>(), U[k]));
        std::remove_copy_if(
            U.begin(), U.end(), std::back_inserter(L),
            std::bind2nd(std::greater_equal<boost::uint16_t>(), U[k]));

        delta_rho = G.size();
        delta_s = 0.0;

        for (boost::uint16_t i = 0; i < G.size(); ++i) {
            delta_s += p_vec[G[i]];
        }

        if ((s + delta_s) - (rho + delta_rho) * p_vec[U[k]] < p_z) {
            s += delta_s;
            rho += delta_rho;

            U.clear();
            std::copy(L.begin(), L.end(), std::back_inserter(U));
        } else {
            U.erase(U.begin() + k);
        }

        G.clear();
        L.clear();
    }

    theta = (s - p_z) / rho;

    for (boost::uint16_t i = 0; i < p_n; ++i) {
        p_vec[i] = std::max(p_vec[i] - theta, 0.0);
    }
}

}
}
