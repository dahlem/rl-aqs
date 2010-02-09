// Copyright (C) 2007-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#include <gsl/gsl_math.h>

#include "Top.hh"
namespace dcommon = des::common;



dcommon::Top::Top()
    : m_maxTS(0.0), m_minTS(DBL_MAX), m_topStart(0.0), m_fifo(new dcommon::EntryList())
{
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

    if (m_fifo != NULL) {
        delete m_fifo;
    }
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


double dcommon::Top::getMaxTS()
{
    return m_maxTS;
}


double dcommon::Top::getMinTS()
{
    return m_minTS;
}


double dcommon::Top::getTopStart()
{
    return m_topStart;
}


boost::uint32_t dcommon::Top::getNTop()
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


bool dcommon::Top::push(dcommon::Entry *p_entry) throw (dcommon::QueueException)
{
#ifdef HAVE_LADDERSTATS
    events_in++;
#endif /* HAVE_LADDERSTATS */

    setMaxTS(p_entry->getArrival());
    setMinTS(p_entry->getArrival());

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


dcommon::EntryList* dcommon::Top::delist()
{
    double width = 1.0;

    if (m_maxTS != m_minTS) {
        width = (gsl_fcmp(m_maxTS, m_minTS, 1e-9) <= 0.0) ? (0.0) : (m_maxTS - m_minTS);
        width /= static_cast<double> (m_fifo->size());
    }

    m_topStart = m_maxTS + width;

    return m_fifo;
}
