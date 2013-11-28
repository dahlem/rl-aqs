// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file LossPolicy.hh
 * Declaration of the abstract interface of the loss policy for neural networks.
 */
#ifndef __LOSS_POLICY_HH__
#define __LOSS_POLICY_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "nnet.hh"


namespace des
{
namespace nnet
{


class LossPolicy
{
public:
    LossPolicy()
        {}

    virtual ~LossPolicy()
        {}

    virtual double error() = 0;

    virtual double addError(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size) = 0;

    virtual double error(DoubleSA p_targets, DoubleSA p_outputs, boost::uint16_t p_size) = 0;
};


typedef boost::shared_ptr <LossPolicy> LossPolicySP;

}
}

#endif /* __LOSS_POLICY_HH__ */
