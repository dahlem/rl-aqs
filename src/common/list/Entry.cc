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

/** @file Entry.cc
 * Implementation of the @ref{Entry.hh} declaration.
 */
#include "Entry.hh"
namespace dcommon = des::common;


boost::uintmax_t dcommon::Entry::uid = 0;


dcommon::Entry::Entry(double del, double a, int d, int o, int t)
    : delay(del), arrival(a), destination(d), origin(o), type(t)
{
    uid++;
    id = uid;
    gid = uid;
    event_path = StackIntSP(new StackInt());
}


dcommon::Entry::Entry(const Entry &p_entry)
    : boost::intrusive::list_base_hook<>(p_entry)
{
    uid++;
    gid = uid;
    id = p_entry.getId();
    delay = p_entry.getDelay();
    arrival = p_entry.getArrival();
    destination = p_entry.getDestination();
    origin = p_entry.getOrigin();
    type = p_entry.getType();
    event_path = p_entry.getEventPath();
}


bool dcommon::Entry::operator< (const dcommon::Entry& rhs) const
{
    if (arrival == rhs.arrival) {
        return gid < rhs.gid;
    } else {
        return arrival < rhs.arrival;
    }
}


bool dcommon::Entry::operator< (const dcommon::Entry& rhs)
{
    if (arrival == rhs.arrival) {
        return gid < rhs.gid;
    } else {
        return arrival < rhs.arrival;
    }
}


void dcommon::Entry::delayed(double p_delay, double p_newArrival, boost::int32_t p_type)
{
    // update the delay
    delay += p_delay;

    // reschedule the arrival
    arrival = p_newArrival;

    type = p_type;
    origin = destination;
}


void dcommon::Entry::service(double p_departure, boost::int32_t p_type)
{
    // set the departure time
    arrival = p_departure;

    // set the departure event type
    type = p_type;

    origin = destination;
}

void dcommon::Entry::depart(boost::int32_t p_destination, boost::int32_t p_type)
{
    destination = p_destination;

    type = p_type;
}


void dcommon::Entry::acknowledge(boost::int32_t p_origin,
                                 boost::int32_t p_destination,
                                 boost::int32_t p_type)
{
    destination = p_destination;
    origin = p_origin;

    type = p_type;
}


void dcommon::Entry::leave(boost::int32_t p_destination, boost::int32_t p_type)
{
    origin = destination;
    destination = p_destination;

    type = p_type;
}


double dcommon::Entry::getDelay() const
{
    return delay;
}

boost::uintmax_t dcommon::Entry::getId() const
{
    return id;
}

double dcommon::Entry::getArrival() const
{
    return arrival;
}

int dcommon::Entry::getDestination() const
{
    return destination;
}

int dcommon::Entry::getOrigin() const
{
    return origin;
}

int dcommon::Entry::getType() const
{
    return type;
}

void dcommon::Entry::push(int origin)
{
    event_path->push(origin);
}

int dcommon::Entry::pop()
{
    int dest = event_path->top();
    event_path->pop();

    return dest;
}

dcommon::StackIntSP dcommon::Entry::getEventPath() const
{
    return event_path;
}
