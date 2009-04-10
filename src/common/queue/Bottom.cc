// Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file Bottom.cc
 * Implementation of the bottom structure @ref{Bottom.hh} of the Ladder Queue.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_QUEUE
# include <iostream>
# include <iomanip>
#endif /* NDEBUG_QUEUE */

#ifdef HAVE_LADDERSTATS
# include <iostream>
# include <ostream>
# include <string>

# include <boost/iostreams/stream.hpp>
namespace bio = boost::iostreams;

# include <boost/shared_ptr.hpp>
#endif /* HAVE_LADDERSTATS */

#include <cstddef>

#include "Bottom.hh"
#include "Entry.hh"
namespace dcommon = des::common;


dcommon::Bottom::Bottom()
    : m_lastEvent(0.0), m_list(new dcommon::EntryList())
{
#ifdef HAVE_LADDERSTATS
    events_in = 0;
    events_out = 0;

    std::string bottomfile = "./bottom-stats.txt";

    // create a buffer
    buf = tStrBufSP(new str_buf(bottomfile.c_str()));

    // create a output stream
    os = tOstreamSP(new std::ostream(buf.get()));
    (*os.get()) << "EventsIn,EventsOut,NumEvents"  << std::endl;
#endif /* HAVE_LADDERSTATS */
}


dcommon::Bottom::~Bottom()
{
    m_list->erase_and_dispose(m_list->begin(), m_list->end(),
                              dcommon::delete_disposer());

    delete m_list;
}


#ifdef HAVE_LADDERSTATS
void dcommon::Bottom::record()
{
    (*os.get()) << events_in << "," << events_out << "," << size() << std::endl;

    // reset the stat fields
    events_in = 0;
    events_out = 0;
}
#endif /* HAVE_LADDERSTATS */


const boost::uint32_t dcommon::Bottom::size()
{
    return static_cast<boost::uint32_t> (m_list->size());
}


/**
 * Insert an entry into the bottom structure. Since the bottom structure
 * maintains a sorted list, this enqueue operation uses an insertion algorithm
 * starting from the back of the queue in order to maintain stability. Otherwise,
 * the bottom structure would not offer stability characteristics.
 *
 * @see Queue#push(dcommon::Entry) throw (QueueException)
 */
const bool dcommon::Bottom::push(dcommon::Entry *p_entry) throw (dcommon::QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << std::setprecision(14) << "Bottom -- Push event: " << const_cast <const dcommon::Entry&> (*p_entry)
              << std::endl;
#endif /* NDEBUG_EVENTS */
    bool inserted = false;

    if (p_entry->getArrival() < m_lastEvent) {
#ifndef NDEBUG_QUEUE
        std::cout << std::setprecision(14) << "Bottom -- PAST event! Last dequeued event: "
                  << m_lastEvent << std::endl;
#endif /* NDEBUG_QUEUE */
        throw dcommon::QueueException(
            dcommon::QueueException::PAST_EVENT_NOT_ALLOWED);
    }

    if (m_list->empty()) {
        // insert straight-away
        m_list->push_back(*p_entry);
#ifndef NDEBUG_QUEUE
        std::cout << "Bottom -- Inserted at the back." << std::endl;
#endif /* NDEBUG_EVENTS */
        inserted = true;
    } else {
        // if the last element is already <= the new entry then insert at the back
        if (m_list->back().getArrival() <= p_entry->getArrival()) {
            m_list->push_back(*p_entry);
#ifndef NDEBUG_QUEUE
            std::cout << "Bottom -- Inserted at the back." << std::endl;
#endif /* NDEBUG_EVENTS */
            inserted = true;
        } else {
            // insertion sort from the back
            // maintain stability
            dcommon::EntryList::reverse_iterator it(m_list->rbegin()), itend(m_list->rend());

            for(; it != itend; ++it) {
                if (it->getArrival() <= p_entry->getArrival()) {
                    dcommon::EntryList::iterator pos = m_list->s_iterator_to(*it);
                    pos++;
#ifndef NDEBUG_QUEUE
                    dcommon::EntryList::iterator pos_start = m_list->s_iterator_to(*it);
                    std::cout << "Bottom -- Inserted between " << pos_start->getArrival()
                              << " and " << pos->getArrival() << std::endl;
#endif /* NDEBUG_EVENTS */
                    m_list->insert(pos, *p_entry);
                    inserted = true;
                    break;
                }
            }
        }
    }

    // worst case scenario: we ended up iterating from back to front without inserting
    if (!inserted) {
        m_list->push_front(*p_entry);
#ifndef NDEBUG_QUEUE
        std::cout << "Bottom -- Inserted at the front." << std::endl;
#endif /* NDEBUG_EVENTS */
        inserted = true;
    }

#ifdef HAVE_LADDERSTATS
    events_in++;
#endif /* HAVE_LADDERSTATS */

    return inserted;
}

/**
 * Insert a list into the bottom structure. If the size of the list to be
 * inserted is less than 10 elements, an insertion sort algorithm is employed.
 * Otherwise, a merge sort algorithm is used.
 *
 * @see List#enlist(node_double_t*, long)
 */
void dcommon::Bottom::push(dcommon::EntryList* p_list)
{
    // this sort does NOT seem to be stable at all
    p_list->sort();

#ifndef NDEBUG_QUEUE
    std::cout << "Bottom -- Push list. Size: " << p_list->size() << std::endl;
    dcommon::Entry *ef = reinterpret_cast<dcommon::Entry*>(&p_list->front());
    dcommon::Entry *eb = reinterpret_cast<dcommon::Entry*>(&p_list->back());
    std::cout << "from: " << ef->getArrival() << " to: " << eb->getArrival() << std::endl;
#endif /* NDEBUG_EVENTS */

    m_list->merge(*p_list);

#ifndef NDEBUG_QUEUE
    std::cout << "Bottom -- Merged lists. Size: " << m_list->size() << std::endl;
    ef = reinterpret_cast<dcommon::Entry*>(&m_list->front());
    eb = reinterpret_cast<dcommon::Entry*>(&m_list->back());
    std::cout << "from: " << ef->getArrival() << " to: " << eb->getArrival() << std::endl;
#endif /* NDEBUG_EVENTS */

#ifdef HAVE_LADDERSTATS
    events_in += p_list->size();
#endif /* HAVE_LADDERSTATS */
}


dcommon::EntryList* const dcommon::Bottom::list()
{
    return m_list;
}

dcommon::Entry* dcommon::Bottom::front() throw (dcommon::QueueException)
{
    if (m_list->size() == 0) {
        throw dcommon::QueueException(
            dcommon::QueueException::QUEUE_EMPTY);
    }

    return reinterpret_cast<dcommon::Entry*>(&m_list->front());
}

void dcommon::Bottom::pop_front() throw (dcommon::QueueException)
{
    if (m_list->size() == 0) {
        throw dcommon::QueueException(
            dcommon::QueueException::QUEUE_EMPTY);
    }

#ifdef HAVE_LADDERSTATS
    events_out++;
#endif /* HAVE_LADDERSTATS */

    // record the last event time stamp
    m_lastEvent = m_list->front().getArrival();

    m_list->pop_front();
}

const double dcommon::Bottom::getMaxTS()
{
    if (!m_list->empty()) {
        return m_list->back().getArrival();
    }

    return m_lastEvent;
}

const double dcommon::Bottom::getMinTS()
{
    if (!m_list->empty()) {
        return m_list->front().getArrival();
    }

    return m_lastEvent;
}
