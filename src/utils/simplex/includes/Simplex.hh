// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spesimplexal exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Simplex.hh
 * Header file with the declaration of utility methods for simplex operations
 */
#ifndef __DES_UTILS_SIMPLEX_HH__
#define __DES_UTILS_SIMPLEX_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <string>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>

#include <gsl/gsl_rng.h>

#include "CRN.hh"
namespace dsample = des::sampling;


typedef boost::shared_array <double> DoubleSA;


namespace des
{
namespace utils
{


/**
 * This class provides simplex utilities
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class Simplex
{
public:
    /** @fn double dotproduct(int, double*, double*)
     * This function implements an efficient euclidean projection onto
     * the l_1 ball. This algorithm is taken from:
     * John Duchi, Shai Shalev-Shwartz, Yoram Singer, Tushar Chandra,
     * "Efficient Projections onto the l_{1}-Ball for Learning in High Dimensions",
     * Proceedings of the 25th International Conference on Machine Learning, Helsinki,
     * Finland, 2009
     *
     * @param int the length of the simplexs
     * @param DoubleSA the left simplex
     * @param DoubleSA the right simplex
     */
    static void projectionDuchi(int, DoubleSA, double, double, dsample::tGslRngSP);
    static void projectionDuchi(int, DoubleSA, double);

private:

    Simplex()
        {}

    ~Simplex()
        {}

};


    }
}


#endif
