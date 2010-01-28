// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file SlidingWindowLossPolicy.hh
 * Declaration/Implementation of the slidingWindow objective function for neural networks. Here
 * the mean squared error is implemented.
 */
#ifndef __SLIDINGWINDOW_LOSS_POLICY_HH__
#define __SLIDINGWINDOW_LOSS_POLICY_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <numeric>
#include <iostream>

#include <boost/cstdint.hpp>
#include <boost/circular_buffer.hpp>


#include "nnet.hh"
#include "LossPolicy.hh"


namespace des
{
namespace nnet
{


typedef boost::circular_buffer<double> CircularBuffer;


class SlidingWindowLossPolicy : public LossPolicy
{
public:
    SlidingWindowLossPolicy(int p_window)
        : LossPolicy(), m_circBuffer(p_window)
        {}

    ~SlidingWindowLossPolicy()
        {
            m_circBuffer.clear();
        }

    inline
    double error()
        {
            if (m_circBuffer.size() == 0) {
                return 0.0;
            } else {
                return std::accumulate(m_circBuffer.begin(), m_circBuffer.end(), 0.0)
                    / static_cast<double> (m_circBuffer.size());
            }
        }

    double addError(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double d = 0.0;
            double error = 0.0;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                d = p_targets[i] - p_outputs[i];
                error += d * d;
            }
            m_circBuffer.push_back(error);

            return std::accumulate(m_circBuffer.begin(), m_circBuffer.end(), 0.0)
                / static_cast<double> (m_circBuffer.size());
        }

    double error(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double d = 0.0;
            double sse = std::accumulate(m_circBuffer.begin(), m_circBuffer.end(), 0.0);
            boost::uint32_t size = m_circBuffer.size() + 1;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                d = p_targets[i] - p_outputs[i];
                sse += d * d;
            }

            return sse / static_cast<double> (size);
        }

private:
    CircularBuffer m_circBuffer;


};

}
}

#endif /* __SLIDINGWINDOW_LOSS_POLICY_HH__ */
