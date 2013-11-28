// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Statistics.hh
 * Declaration/Implementation of the network for neural networks
 */
#ifndef __DES_NET_STATISTICS_HH__
#define __DES_NET_STATISTICS_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>


namespace des
{
namespace nnet
{

class Statistics
{
private:
    Statistics()
        {
        }

    ~Statistics()
        {}

public:
    inline
    static double error(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double error = 0.0;
            double temp = 0.0;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                temp = p_targets[i] - p_outputs[i];
                error += temp * temp;
            }

            return error;
        }
};

}
}



#endif /* __DES_NET_STATISTICS_HH__ */
