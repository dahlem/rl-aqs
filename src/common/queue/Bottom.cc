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

#include <cstddef>

#include "Bottom.hh"
using des::common::Bottom;

#include "Mergesort.hh"
using des::common::Mergesort;



Bottom::Bottom()
{
    m_lastEvent = 0.0;

    init();
}

Bottom::~Bottom()
{
    node_double_t *current = m_head->next;

    while (current) {
        // if tail node
        if (current->data == NULL) break;

        node_double_t *deleteNode = current;

        // advance the current pointer
        current = current->next;

        // delete and the node
        delete deleteNode;
    }

    delete m_head;
    delete m_tail;
}

const long Bottom::size()
{
    return m_size;
}

void Bottom::init()
{
    m_head = new node_double_t(NULL, NULL, NULL);
    m_tail = new node_double_t(NULL, NULL, NULL);

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
 * @see Queue#enqueue(entry_t*) throw (QueueException)
 */
void Bottom::enqueue(entry_t *const p_entry) throw (QueueException)
{
    if (p_entry == NULL) return;

    if (p_entry->arrival < m_lastEvent) {
        throw QueueException(QueueException::PAST_EVENT_NOT_ALLOWED);
    }

    node_double_t *temp = m_tail->previous;

    // insertion sort from the back
    // maintain stability
    while (temp != NULL) {
        if ((temp->data == NULL)
            || (temp->data->arrival <= p_entry->arrival)) {
            node_double_t *newEntry = new node_double_t(
                p_entry, temp->next, temp);

            temp->next->previous = newEntry;
            temp->next = newEntry;
            m_size++;

            return;
        }

        temp = temp->previous;
    }
}

/**
 * Insert a list into the bottom structure. If the size of the list to be
 * inserted is less than 10 elements, an insertion sort algorithm is employed.
 * Otherwise, a merge sort algorithm is used.
 *
 * @see List#enlist(node_double_t*, long)
 */
void Bottom::enlist(node_double_t *p_list, long p_size)
{
    // insert data items of a small list individually, otherwise use mergesort
    if (p_size < 10) {

        for (int i = 0; i < p_size; ++i) {
            enqueue(p_list->data);
            p_list = p_list->next;
        }
    } else {
        p_list = Mergesort::sort(p_list);
        node_double_t *t = Mergesort::merge(m_head->next, p_list);

        t->previous = m_head;
        m_head->next = t;

        m_size += p_size;
    }
}

node_double_t *Bottom::delist()
{
    node_double_t *list = m_head;

    // re-initialise the fifo data structure
    init();

    // return the current list
    return list;
}

entry_t *const Bottom::dequeue()
{
    if (m_size == 0) {
        return NULL;
    }

    node_double_t *temp = m_head->next;
    entry_t *const result = temp->data;

    m_head->next = temp->next;
    temp->next->previous = m_head;

    delete temp;
    m_size--;

    m_lastEvent = result->arrival;

    return result;
}

double Bottom::getMaxTS()
{
    if (m_size != 0) {
        return m_tail->previous->data->arrival;
    }

    return m_lastEvent;
}

double Bottom::getMinTS()
{
    if (m_size != 0) {
        return m_head->next->data->arrival;
    }

    return m_lastEvent;
}
