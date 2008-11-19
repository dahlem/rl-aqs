// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef __EVENTGENERATOR_HH__
#define __EVENTGENERATOR_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include "CRN.hh"
namespace dsample = des::sampling;

#include "LadderQueue.hh"
namespace dcommon = des::common;




namespace des { namespace core {



/**
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class EventGenerator
{
private:
    EventGenerator();
    ~EventGenerator();

public:
    static void generate(
        dcommon::tQueueSP p_queue,
        dsample::tGslRngSP arrival_rng,
        boost::int32_t destination,
        double arrival_rate,
        double stop_time);

    static void generate(
        dcommon::tQueueSP p_queue,
        dsample::tGslRngSP arrival_rng,
        boost::int32_t destination,
        double arrival_rate);

};

    }
}

#endif
