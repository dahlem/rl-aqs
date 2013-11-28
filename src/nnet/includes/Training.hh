// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Training.hh
 * abstract Declaration of the training interface.
 */
#ifndef __NNET_TRAINING_HH__
#define __NNET_TRAINING_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <boost/shared_ptr.hpp>


#include "nnet.hh"


namespace des
{
namespace nnet
{


class Training
{
public:
    Training()
        {}


    virtual ~Training()
        {}

    virtual void train(DoubleSA p_targets) = 0;
};


typedef boost::shared_ptr <Training> TrainingSP;


}
}

#endif
