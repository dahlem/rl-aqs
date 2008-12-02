// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

template <class NeuralNetwork, class Objective>
class Statistics
{
public:
    Statistics(NeuralNetwork p_nnet)
        : m_nnet(p_nnet)
        {
        }

    ~Statistics()
        {}

    inline
    double error(DoubleSA p_targets)
        {
            return  Objective::error(p_targets,
                                     m_nnet->getOutputNeurons(),
                                     m_nnet->getNumOutputs());

        }

private:
    NeuralNetwork m_nnet;
};

}
}



#endif /* __DES_NET_STATISTICS_HH__ */
