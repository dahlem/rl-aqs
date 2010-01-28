// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file DefaultLossPolicy.hh
 * Declaration/Implementation of the default objective function for neural networks. Here
 * the mean squared error is implemented.
 */
#ifndef __DEFAULT_LOSS_POLICY_HH__
#define __DEFAULT_LOSS_POLICY_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>


#include "nnet.hh"
#include "LossPolicy.hh"


namespace des
{
namespace nnet
{


class DefaultLossPolicy : public LossPolicy
{
public:
    DefaultLossPolicy()
        : LossPolicy(), m_sse(0.0), m_size(0)
        {}

    ~DefaultLossPolicy()
        {}

    inline
    double error()
        {
            if (m_size == 0) {
                return 0.0;
            } else {
                return m_sse / static_cast<double> (m_size);
            }
        }

    double addError(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double d = 0.0;
            m_size++;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                d = p_targets[i] - p_outputs[i];
                m_sse += d * d;
            }

            return m_sse / static_cast<double> (m_size);
        }

    double error(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size)
        {
            double d = 0.0;
            double sse = m_sse;
            boost::uint32_t size = m_size + 1;

            for (boost::uint16_t i = 0; i < p_size; ++i) {
                d = p_targets[i] - p_outputs[i];
                sse += d * d;
            }

            return sse / static_cast<double> (size);
        }

private:
    double m_sse;
    boost::uint32_t m_size;

};

}
}

#endif /* __DEFAULT_LOSS_POLICY_HH__ */
