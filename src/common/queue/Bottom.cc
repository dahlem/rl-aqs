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

/** @file Bottom.cc
 * Implementation of the bottom structure @ref{Bottom.hh} of the Ladder Queue.
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

#include "Bottom.hh"
#include "Entry.hh"
#include "Mergesort.hh"
namespace dcommon = des::common;



dcommon::Bottom::Bottom()
{
    m_lastEvent = 0.0;

    init();

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
    dcommon::node_double_t *current = m_head->next;

    while (current) {
        // if tail node
        if (current->data == NULL) break;

        dcommon::node_double_t *deleteNode = current;

        // advance the current pointer
        current = current->next;

        // delete and the node
        delete deleteNode;
    }

    delete m_head;
    delete m_tail;
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


const long dcommon::Bottom::size()
{
    return m_size;
}


void dcommon::Bottom::init()
{
    dcommon::entry_t *entry = NULL;
    m_head = new dcommon::node_double_t(dcommon::tEntrySP(entry), NULL, NULL);
    m_tail = new dcommon::node_double_t(dcommon::tEntrySP(entry), NULL, NULL);

    m_head->next = m_tail;
    m_tail->previous = m_head;

    m_size = 0;
}


/**
 * Insert an entry into the bottom structure. Since the bottom structure
 * maintains a sorted list, this enqueue operation uses an insertion algorithm
 * starting from the back of the queue in order to maintain stability. Otherwise,
 * the bottom structure would not offer stability characteristics.
 *
 * @see Queue#enqueue(dcommon::tEntrySP) throw (QueueException)
 */
void dcommon::Bottom::enqueue(dcommon::tEntrySP p_entry) throw (dcommon::QueueException)
{
    if (p_entry == NULL) return;

    if (p_entry->arrival < m_lastEvent) {
        throw dcommon::QueueException(
            dcommon::QueueException::PAST_EVENT_NOT_ALLOWED);
    }

    dcommon::node_double_t *temp = m_tail->previous;

    // insertion sort from the back
    // maintain stability
    while (temp != NULL) {
        if ((temp->data == NULL)
            || (temp->data->arrival <= p_entry->arrival)) {
            dcommon::node_double_t *newEntry = new dcommon::node_double_t(
                p_entry, temp->next, temp);

            temp->next->previous = newEntry;
            temp->next = newEntry;
            m_size++;

            return;
        }

        temp = temp->previous;
    }

#ifdef HAVE_LADDERSTATS
    events_in++;
#endif /* HAVE_LADDERSTATS */
}

/**
 * Insert a list into the bottom structure. If the size of the list to be
 * inserted is less than 10 elements, an insertion sort algorithm is employed.
 * Otherwise, a merge sort algorithm is used.
 *
 * @see List#enlist(node_double_t*, long)
 */
void dcommon::Bottom::enlist(dcommon::node_double_t *p_list, long p_size)
{
    // insert data items of a small list individually, otherwise use mergesort
    if (p_size < 10) {

        for (int i = 0; i < p_size; ++i) {
            enqueue(p_list->data);
            p_list = p_list->next;
        }
    } else {
        p_list = dcommon::Mergesort::sort(p_list);
        dcommon::node_double_t *t = dcommon::Mergesort::merge(m_head->next, p_list);

        t->previous = m_head;
        m_head->next = t;

        m_size += p_size;

#ifdef HAVE_LADDERSTATS
        events_in += p_size;
#endif /* HAVE_LADDERSTATS */
    }
}


dcommon::node_double_t *dcommon::Bottom::delist()
{
    dcommon::node_double_t *list = m_head;

    // re-initialise the fifo data structure
    init();

    // return the current list
    return list;
}

dcommon::tEntrySP dcommon::Bottom::dequeue()
{
    if (m_size == 0) {
        dcommon::entry_t *entry = NULL;
        return dcommon::tEntrySP(entry);
    }

    dcommon::node_double_t *temp = m_head->next;
    dcommon::tEntrySP result = temp->data;

    m_head->next = temp->next;
    temp->next->previous = m_head;

    delete temp;
    m_size--;

    m_lastEvent = result->arrival;

#ifdef HAVE_LADDERSTATS
    events_out++;
#endif /* HAVE_LADDERSTATS */

    return result;
}

double dcommon::Bottom::getMaxTS()
{
    if (m_size != 0) {
        return m_tail->previous->data->arrival;
    }

    return m_lastEvent;
}

double dcommon::Bottom::getMinTS()
{
    if (m_size != 0) {
        return m_head->next->data->arrival;
    }

    return m_lastEvent;
}
