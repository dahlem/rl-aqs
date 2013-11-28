// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#ifndef LHS_HH
#define LHS_HH

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>


#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "SamplingException.hh"
using des::sampling::SamplingException;


namespace des
{
    namespace sampling
    {

/**
 * This class implements Latin Hypercube Sampling with re-scaling.
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class LHS
{
private:
    /**
     * Hide the constructors, because we don't want an instance to be created.
     */
    LHS();
    ~LHS();

public:

    /**
     * Get the latin hypercube sample.
     *
     * @param const gsl_rng* the random number generator
     * @param const gsl_vector* the vector with the minimum values for each dimension
     * @param const gsl_vector* the vector with the maximum values for each dimension
     * @param int the sample size
     * @param gsl_matrix** the obtained sample
     * @throws SamplingException, if an error occurs during the sampling.
     */
    static void sample(const gsl_rng *p_rng, const gsl_vector *p_min,
                       const gsl_vector *p_max, int p_size, gsl_matrix **p_mat)
        throw (SamplingException);


    /**
     * Get the optimal latin hypercube sample according to Ye.
     *
     * @param const gsl_rng* the random number generator
     * @param const gsl_vector* the vector with the minimum values for each dimension
     * @param const gsl_vector* the vector with the maximum values for each dimension
     * @param int the sample size
     * @param gsl_matrix** the obtained sample
     * @throws SamplingException, if an error occurs during the sampling.
     */
    static void sample(const gsl_rng *p_rng, const gsl_vector *p_min,
                       const gsl_vector *p_max, int p_size, gsl_matrix **p_mat,
                       const int p_r)
        throw (SamplingException);


    static void matrixM(int p_m, gsl_vector &p_e, gsl_matrix **p_M);
    static void matrixS(int p_m, gsl_matrix **p_S);
    static void euclideanDistance(gsl_matrix &p_mat, gsl_vector **p_dist);
    static void uniqueInts(const gsl_rng *p_rng, boost::uint32_t *r1, boost::uint32_t *r2,
                           boost::uint32_t maxVal);


    static void kronecker(const gsl_matrix &p_a, const gsl_matrix &p_b,
                          gsl_matrix **p_mat);


};


    }
}

#endif
