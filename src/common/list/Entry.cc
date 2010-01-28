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

/** @file Entry.cc
 * Implementation of the @ref{Entry.hh} declaration.
 */
#include "Entry.hh"

#include <iostream>


namespace des
{
namespace common
{


boost::uintmax_t Entry::uid = 0;


Entry::Entry()
    : delay(0.0), arrival(0.0), externalArrival(0), destination(0), origin(0), type(0),
      event_path(), event_arrivals()
{
    uid++;
    id = uid;
    gid = uid;
}

Entry::Entry(double del, double a, int d, int o, int t)
    : delay(del), arrival(a), externalArrival(a), destination(d), origin(o), type(t),
      event_path(), event_arrivals()
{
    uid++;
    id = uid;
    gid = uid;
}


Entry::Entry(const Entry &p_entry)
    : boost::intrusive::list_base_hook<>(),
      id(p_entry.id), delay(p_entry.delay), arrival(p_entry.arrival),
      externalArrival(p_entry.externalArrival),
      destination(p_entry.destination), origin(p_entry.origin), type(p_entry.type),
      event_path(p_entry.event_path),
      event_arrivals(p_entry.event_arrivals)
{
    uid++;
    gid = uid;
}

Entry::~Entry()
{
}


bool Entry::operator< (const Entry& rhs) const
{
    if (arrival == rhs.arrival) {
        return gid < rhs.gid;
    } else {
        return arrival < rhs.arrival;
    }
}


bool Entry::operator< (const Entry& rhs)
{
    if (arrival == rhs.arrival) {
        return gid < rhs.gid;
    } else {
        return arrival < rhs.arrival;
    }
}


void Entry::delayed(double p_delay, double p_newArrival, boost::int32_t p_type)
{
    // update the delay
    delay += p_delay;

    // reschedule the arrival
    arrival = p_newArrival;

    type = p_type;
    origin = destination;
}


void Entry::service(double p_departure, boost::int32_t p_type)
{
    // set the departure time
    arrival = p_departure;

    // set the departure event type
    type = p_type;

    origin = destination;
}


void Entry::depart(boost::int32_t p_destination, boost::int32_t p_type)
{
    destination = p_destination;

    type = p_type;
}


void Entry::acknowledge(boost::int32_t p_origin,
                                 boost::int32_t p_destination,
                                 boost::int32_t p_type)
{
    destination = p_destination;
    origin = p_origin;

    type = p_type;
}


void Entry::leave(boost::int32_t p_destination, boost::int32_t p_type)
{
    origin = destination;
    destination = p_destination;

    type = p_type;
}


double Entry::getDelay() const
{
    return delay;
}


boost::uintmax_t Entry::getId() const
{
    return id;
}


double Entry::getArrival() const
{
    return arrival;
}

void Entry::setArrival(double p_arrival)
{
    arrival = p_arrival;
}


int Entry::getDestination() const
{
    return destination;
}


int Entry::getOrigin() const
{
    return origin;
}


int Entry::getType() const
{
    return type;
}


void Entry::pushEventHistory(boost::int32_t p_node, double p_time)
{
    event_path.push(p_node);
    event_arrivals.push(p_time);
}

int Entry::getEventHistoryLength()
{
    return event_path.size();
}


int Entry::popEvent()
{
    int dest = event_path.top();
    event_path.pop();
    event_arrivals.pop();

    return dest;
}


bool Entry::isEventQueueEmpty()
{
    return event_path.empty();
}


double Entry::topArrival()
{
    return event_arrivals.top();
}


double Entry::getExternalArrival()
{
    return externalArrival;
}



}
}
