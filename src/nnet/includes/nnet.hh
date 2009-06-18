// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file nnet.hh
 * Declaration of common date types in nnet
 */
#ifndef __NNET_HH__
#define __NNET_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <boost/shared_array.hpp>


namespace des
{
namespace nnet
{


#define ERRTOL          1e-6

template<class T>
inline T SIGN(const T &a, const T &b)
{
    return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}

typedef boost::shared_array <double> DoubleSA;
typedef boost::shared_array <DoubleSA> DoubleSM;


}
}

#endif
