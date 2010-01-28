// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include <gsl/gsl_randist.h>

#include "CRN.hh"
#include "Rng.hh"
namespace dsample = des::sampling;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "EventGenerator.hh"

#include "DirectedGraph.hh"
namespace dnet = des::network;


namespace des
{
namespace core
{


void EventGenerator::generateLogGraphEvent(dcommon::Queue &p_queue, double p_scheduledTime)
{
    dcommon::Entry *entry = new dcommon::Entry(
        0.0,
        p_scheduledTime,
        -99,
        ADMIN_EVENT,
        LOG_GRAPH_EVENT);

#ifndef NDEBUG_EVENTS
    std::cout << "Admin log graph event for time " << time << " scheduled." << std::endl;
#endif /* NDEBUG_EVENTS */

    try {
        p_queue.push(entry);
    } catch (dcommon::QueueException &qe) {
        std::cout << "Error scheduling admin log graph event: " << entry->getArrival() << " " << qe.what() << std::endl;
        if (entry != NULL) {
            delete entry;
        }
        throw;
    }
}


void EventGenerator::generateArrivalAdmin(
    dcommon::Queue &p_queue,
    boost::int32_t p_destination,
    double p_time)
{

    // enqueue the last arrival event
    dcommon::Entry *entry = new dcommon::Entry(
        0.0, // delay
        p_time, // arrival
        p_destination, // destination
        ADMIN_EVENT, // origin
        GENERATE_ARRIVAL_EVENT); // type

#ifndef NDEBUG_EVENTS
    std::cout << "Admin event for external arrival at time " << p_time
              << " scheduled for vertex " << p_destination << std::endl;
#endif /* NDEBUG_EVENTS */

    try {
        p_queue.push(entry);
    } catch (dcommon::QueueException &qe) {
        std::cout << "Error scheduling admin event for external arrival: " << entry->getArrival() << " " << qe.what() << std::endl;
        if (entry != NULL) {
            delete entry;
        }
        throw;
    }
}


void EventGenerator::generateArrival(
    dcommon::Queue &p_queue,
    dsample::tGslRngSP arrival_rng,
    boost::int32_t destination,
    double start_time,
    double arrival_rate)
{
    double cur_arrival = -dsample::Rng::poiss(
        arrival_rate, gsl_rng_uniform(arrival_rng.get()));

    cur_arrival += start_time;

    // enqueue the last arrival event
    dcommon::Entry *entry = new dcommon::Entry(
        0.0,
        cur_arrival,
        destination,
        EXTERNAL_EVENT,
        LAST_ARRIVAL_EVENT);

    try {
        p_queue.push(entry);
#ifndef NDEBUG_EVENTS
        std::cout << "External last arrival event scheduled for vertex " << destination << std::endl;
#endif /* NDEBUG_EVENTS */
    } catch (dcommon::QueueException &qe) {
        std::cout << "Error scheduling external last arrival event: " << entry->getArrival() << " " << qe.what() << std::endl;
        if (entry != NULL) {
            delete entry;
        }
        throw;
    }
}


}
}
