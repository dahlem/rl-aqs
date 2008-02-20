// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SCF_HH
#define SCF_HH

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "StatsException.hh"
using des::statistics::StatsException;



namespace des
{
    namespace statistics
    {


/**
 * This class calculates spatial correlation.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class SCF
{
public:
    /**
     * The spatial correlation function supports gaussian and exponential correlation
     * functions determined by the exponent of the differences. For \$ 1 \le p \lt 2 \$
     * The scf corresponds to the exponential correlation function, while for
     * \$ p = 2 \$ corresponds the gaussian correlation function. Theta controls the
     * range of influence.
     *
     * @param const gsl_vector const* the x vector
     * @param const gsl_vector const* the y vector
     * @param const gsl_vector const* the theta vector
     * @param const double the exponent
     * @param const double the nugget effect
     * @return the correlation value
     * @throws StatsException, if the correlation function cannot be calculated.
     */
    static double scf_func(
        const gsl_vector *const p_x,
        const gsl_vector *const p_y,
        const gsl_vector *const p_theta,
        const double p_exp,
        const double p_nugget) throw (StatsException);

    /**
     * The spatial correlation function calculates the correlation matrix.
     *
     * @param const gsl_matrix const* the X matrix
     * @param gsl_matrix* the correlation matrix R
     * @param const gsl_vector const* the theta vector
     * @param const double the exponent
     * @param const double the nugget effect
     * @throws StatsException, if the correlation matrix cannot be calculated.
     */
    static void scf(
        const gsl_matrix *const p_X,
        gsl_matrix *const p_R,
        const gsl_vector *const p_theta,
        const double p_exp,
        const double p_nugget) throw (StatsException);

private:
    SCF()
        {}

    ~SCF()
        {}

};


    }
}




#endif
