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
#include <iostream>

#include "Entry.hh"
namespace dcommon = des::common;


boost::uintmax_t dcommon::Entry::uid = 0;


bool dcommon::Entry::operator< (const dcommon::Entry& rhs) const
{
    return arrival < rhs.arrival;
}


bool dcommon::Entry::operator< (const dcommon::Entry& rhs)
{
    return arrival < rhs.arrival;
}


std::ostream& dcommon::operator <<(std::ostream &p_os, const dcommon::Entry &p_entry)
{
    p_os << p_entry.uid << "," << p_entry.id << "," << p_entry.arrival << ","
         << p_entry.destination << "," << p_entry.origin << "," << p_entry.type;

    return p_os;
}


std::ostream& dcommon::operator <<(std::ostream &p_os, dcommon::Entry &p_entry)
{
    p_os << p_entry.uid << "," << p_entry.id << "," << p_entry.arrival << ","
         << p_entry.destination << "," << p_entry.origin << "," << p_entry.type;

    return p_os;
}
