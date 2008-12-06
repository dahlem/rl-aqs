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

/** @file CurDate.cc
 * Implementation of the <code>CurDate</code> class. It uses the boost
 * date_time package internally to retrieve and parse the local date.
 */
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
namespace po_time = boost::posix_time;

#include "CurDate.hh"


namespace des
{
namespace date
{


CurDate::CurDate()
{
    po_time::ptime now = po_time::second_clock::local_time();
    std::stringstream ss;

    po_time::time_facet* timefacet = new po_time::time_facet();
    ss.imbue(std::locale(std::locale::classic(), timefacet));
    timefacet->format("%Y%m%d%H%M%S");

    ss << now;
    m_cur_date = ss.str();
}


CurDate::~CurDate()
{}


string CurDate::get() const
{
    return m_cur_date;
}


}
}
