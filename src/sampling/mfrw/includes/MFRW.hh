// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file MFRW.hh
 * Declaration of the multifractal random walk interface.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __SAMPLING_MFRW_HH__
#define __SAMPLING_MFRW_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>


namespace des
{
namespace sampling
{


/** @typedef tGslRngSP
 * Typedef of the shared pointer to a GSL random number generator.
 */
typedef boost::shared_ptr<gsl_rng> tGslRngSP;


/**
 * Implementation of a multifractal random walk. This class implements
 * the multifractal random walk as described by Kan Chen,
 * C. Jayaprakash, Baosheng Yuan in "Conditional Probability as a
 *    Measure of Volatility Clustering in Financial Time Series"
 *    (2006).
 *
 * @author <a href="mailto:Dominik.Dahlem@scss.tcd.ie">Dominik Dahlem</a>
 */
class MFRW
{
public:
    /** @fn path(double, double, double, double, boost::uint16_t, boost::uint16_t, double, double)
     * Sample a multifractal random walk path given
     *
     * @param gsl_vector* vector to store the result
     * @param tGslRngSP the GSL random number generator acc. to binomial dist
     * @param tGslRngSP the GSL random number generator acc. to normal dist
     * @param double d0
     * @param double a0
     * @param double b
     * @param double lambda
     * @param boost::uint16_t Nc
     * @param boost::uint16_t T
     * @param double n0
     * @param double nmax = log(30)/log(lambda)
     */
    static void path(gsl_vector*, tGslRngSP, tGslRngSP, double, double, double, double, boost::uint16_t, boost::uint16_t, double, double);

private:
    MFRW()
        {}

    ~MFRW()
        {}

};

}
}


#endif /* __SAMPLING_MFRW_HH__ */
