// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file MSE.hh
 * Declaration/Implementation of the objective function for neural networks. Here
 * the mean squared error is implemented.
 */
#ifndef __MSE_HH__
#define __MSE_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>


typedef boost::shared_array <double> DoubleSA;


namespace des { namespace nnet {


/** @class MSE
 * This class implements the mean squared error objective function.
 *
 * @author Dominik Dahlem
 */
class MSE
{
public:
    static double error(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double d, sum = 0.0;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                d = p_targets[i] - p_outputs[i];
                sum += d * d;
            }

            return sum / static_cast<double> (p_size);
        }


private:
    MSE()
        {}

    MSE(const MSE&)
        {}

    ~MSE()
        {}

};


}
}

#endif /* __MSE_HH__ */
