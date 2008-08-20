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


#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_randist.h>

#include "CRN.hh"
#include "Rng.hh"
namespace dsample = des::sampling;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "EventGenerator.hh"
namespace dcore = des::core;


void dcore::EventGenerator::generate(
    boost::shared_ptr <dcommon::LadderQueue> queue,
    dsample::tGslRngSP arrival_rng,
    boost::int32_t destination,
    double arrival_rate,
    double stop_time)
{
    double cur_arrival, new_arrival;

    cur_arrival = -dsample::Rng::poiss(
        arrival_rate, gsl_rng_uniform(arrival_rng.get()));

    // for as long as there is no stopping event
    while (cur_arrival < stop_time) {
        // generate arrival events
        new_arrival = dsample::Rng::poiss(
            arrival_rate, gsl_rng_uniform(arrival_rng.get()));

        if ((cur_arrival - new_arrival) <= stop_time) {
            // enqueue the last arrival event
            dcommon::entry_t *entry = new dcommon::entry_t(
                cur_arrival,
                destination,
                dcore::EXTERNAL_EVENT,
                dcore::ARRIVAL_EVENT);
            queue->enqueue(entry);
            cur_arrival -= new_arrival;
        } else {
            // enqueue the last arrival event
            dcommon::entry_t *entry = new dcommon::entry_t(
                cur_arrival,
                destination,
                dcore::EXTERNAL_EVENT,
                dcore::LAST_ARRIVAL_EVENT);
            queue->enqueue(entry);
            break;
        }
    }

    cur_arrival = 0;
}
