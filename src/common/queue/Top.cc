// Copyright (C) 2007,2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file Top.hh
 * Implementation of the top structure @ref{Top.hh} of the Ladder Queue.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_LADDERSTATS
# include <iostream>
# include <ostream>
# include <string>

# include <boost/iostreams/stream.hpp>
namespace bio = boost::iostreams;

# include <boost/shared_ptr.hpp>
#endif /* HAVE_LADDERSTATS */

#include <cstddef>
#include <cfloat>

#include "Top.hh"
namespace dcommon = des::common;



dcommon::Top::Top()
    : m_maxTS(0.0), m_minTS(DBL_MAX), m_topStart(0.0)
{
    m_fifo = new dcommon::EntryList();

#ifdef HAVE_LADDERSTATS
    events_in = 0;
    events_out = 0;

    std::string topfile = "./top-stats.txt";

    // create a buffer
    buf = tStrBufSP(new str_buf(topfile.c_str()));

    // create a output stream
    os = tOstreamSP(new std::ostream(buf.get()));
    (*os.get()) << "EventsIn,EventsOut,NumEvents"  << std::endl;
#endif /* HAVE_LADDERSTATS */
}


dcommon::Top::~Top()
{
    m_fifo->erase_and_dispose(m_fifo->begin(), m_fifo->end(),
                              dcommon::delete_disposer());

    delete m_fifo;
}


#ifdef HAVE_LADDERSTATS
void dcommon::Top::record()
{
    (*os.get()) << events_in << "," << events_out << "," << getNTop() << std::endl;

    // reset the stat fields
    events_in = 0;
    events_out = 0;
}
#endif /* HAVE_LADDERSTATS */


const double dcommon::Top::getMaxTS()
{
    return m_maxTS;
}


const double dcommon::Top::getMinTS()
{
    return m_minTS;
}


const double dcommon::Top::getTopStart()
{
    return m_topStart;
}


const boost::uint32_t dcommon::Top::getNTop()
{
    return static_cast<boost::uint32_t> (m_fifo->size());
}


void dcommon::Top::reset() throw (dcommon::QueueException)
{
    if (getNTop() > 0) {
        throw dcommon::QueueException(dcommon::QueueException::BAD_RESET);
    } else {
        m_maxTS = 0.0;
        m_minTS = DBL_MAX;
    }
}


void dcommon::Top::setMaxTS(double p_maxTS)
{
    if (p_maxTS > m_maxTS) {
        m_maxTS = p_maxTS;
    }
}


void dcommon::Top::setMinTS(double p_minTS)
{
    if (p_minTS < m_minTS) {
        m_minTS = p_minTS;
    }
}


const bool dcommon::Top::push(dcommon::Entry *p_entry) throw (dcommon::QueueException)
{
#ifdef HAVE_LADDERSTATS
    events_in++;
#endif /* HAVE_LADDERSTATS */

    setMaxTS(p_entry->arrival);
    setMinTS(p_entry->arrival);

    m_fifo->push_back(*p_entry);
    return true;
}


void dcommon::Top::push(dcommon::EntryList* p_list)
{
    dcommon::Entry *entry = NULL;

    while (!p_list->empty()) {
        entry = reinterpret_cast<dcommon::Entry*>(&p_list->front());
        p_list->pop_front();
        push(entry);
    }
}


dcommon::EntryList* const dcommon::Top::delist()
{
    m_topStart = m_maxTS;
    return m_fifo;
}
