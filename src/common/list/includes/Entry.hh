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

/** @file Entry.hh
 * Specification of an entry for the priority queue.
 */
#ifndef ENTRY_HH
#define ENTRY_HH


namespace des
{
    namespace common
    {


/** @struct entry_t
 * This structure specifies an (event) entry for the priority queue.
 * An event is specified by the arrival time, the destination, the origin, and
 * a type of the event.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
struct entry_t
{
    double arrival;
    double service;
    int destination;
    int origin;
    int type;

    entry_t(double a, double s, int d, int o, int t)
        {
            arrival = a;
            service = s;
            destination = d;
            origin = o;
            type = t;
        }

};

    }
}


#endif
