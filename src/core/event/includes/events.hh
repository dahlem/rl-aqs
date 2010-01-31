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

/** @file events.hh
 * Specification of the event types.
 */
#ifndef __EVENTS_HH__
#define __EVENTS_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>


namespace des {namespace core {

/**
 * Declaration of the event types
 */
const boost::int32_t LAST_ARRIVAL_EVENT         = 0;
const boost::int32_t ARRIVAL_EVENT              = 1;
const boost::int32_t RESCHEDULED_EVENT          = 2;
const boost::int32_t DEPARTURE_EVENT            = 3;
const boost::int32_t ACK_EVENT                  = 4;
const boost::int32_t ACK_1HOP_EVENT             = 5;
const boost::int32_t LEAVE_EVENT                = 6;

/**
 * Declaration of admin events
 */
const boost::int32_t LOG_GRAPH_EVENT            = 10;
const boost::int32_t GENERATE_ARRIVAL_EVENT     = 11;

/**
 * Declaration of the event source
 */
const boost::int32_t EXTERNAL_EVENT             = -1;
const boost::int32_t ADMIN_EVENT                = -2;


    }
}


#endif
