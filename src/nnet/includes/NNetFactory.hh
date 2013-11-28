// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file NNetFactory.hh
 * Declaration/Implementation of the NNetFactory
 */
#ifndef __NNETFACTORY_HH__
#define __NNETFACTORY_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>


#include "Backpropagation.hh"
#include "ConjugateGradient.hh"
#include "DefaultLossPolicy.hh"
#include "FeedforwardNetwork.hh"
#include "Identity.hh"
#include "HTangent.hh"
#include "Logistic.hh"
#include "LossPolicy.hh"
#include "MSE.hh"
#include "nnet.hh"
#include "Statistics.hh"
#include "SlidingWindowLossPolicy.hh"
#include "Training.hh"

namespace des
{
namespace nnet
{


class NNetFactory
{
public:

    template <typename Activation, typename ActivationOutput>
    static boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >
    createNNet(boost::uint16_t p_num_inputs, boost::uint16_t p_num_hidden,
               boost::uint16_t p_num_outputs, boost::uint32_t p_uniform_idx)
        {
            return boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >
                (new FeedforwardNetwork <Activation, ActivationOutput>(p_num_inputs, p_num_hidden, p_num_outputs, p_uniform_idx));
        }

    template <typename Activation, typename ActivationOutput>
    static boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> >
    createDefaultMSEObjective(boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> > p_net)
        {
            LossPolicySP policy = LossPolicySP(new DefaultLossPolicy());
            return boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> >
                (new MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput>(p_net, policy));
        }

    template <typename Activation, typename ActivationOutput>
    static boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> >
    createSlidingWindowMSEObjective(boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> > p_net, int p_window)
        {
            LossPolicySP policy = LossPolicySP(new SlidingWindowLossPolicy(p_window));
            return boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> >
                (new MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput>(p_net, policy));
        }

    template <typename Activation, typename ActivationOutput>
    static TrainingSP createConjugateGradientTraining(boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> > p_nnet,
                                                      boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> > p_objective,
                                                      double p_trainingRate,
                                                      double p_errtol,
                                                      boost::uint16_t p_iterMax)
        {
            return TrainingSP(new ConjugateGradient <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> > >
                              (p_nnet, p_objective, p_trainingRate, p_errtol, p_iterMax));
        }

    template <typename Activation, typename ActivationOutput>
    static TrainingSP createBackpropagationTraining(boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> > p_nnet,
                                                    boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> > p_objective,
                                                    double p_trainingRate,
                                                    double p_momentum,
                                                    double p_errtol)
        {
            return TrainingSP(new Backpropagation <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, boost::shared_ptr <MSE <boost::shared_ptr <FeedforwardNetwork <Activation, ActivationOutput> >, Activation, ActivationOutput> > >
                              (p_nnet, p_objective, p_trainingRate, p_momentum, p_errtol));
        }



private:
    NNetFactory()
        {}

    ~NNetFactory()
        {}
};


}
}

#endif
