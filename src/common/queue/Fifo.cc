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

/** @file Fifo.cc
 * Implementation of the FIFO list/queue @ref{Fifo.hh}
 */

#include <cstddef>

#include "Fifo.hh"
namespace dcommon = des::common;



dcommon::Fifo::Fifo()
{
    dcommon::entry_t *entry = NULL;
    m_head = new dcommon::node_double_t(dcommon::tEntrySP(entry), NULL, NULL);
    m_tail = new dcommon::node_double_t(dcommon::tEntrySP(entry), NULL, NULL);

    init();
}


dcommon::Fifo::~Fifo()
{
    if ((m_head != NULL ) && (m_tail != NULL)) {
        dcommon::node_double_t *current = m_head->next;

        while (current != NULL) {
            // if tail node
//            dcommon::tEntrySP entry = current->data;
//            if (entry == NULL) break;

            dcommon::node_double_t *deleteNode = current;

            // advance the current pointer
            current = current->next;

            // delete and the node
            delete deleteNode;
        }

        delete m_head;
//        delete m_tail;
    }

}

void dcommon::Fifo::init()
{
    m_head->next = m_tail;
    m_tail->previous = m_head;
    m_size = 0;
}

const long dcommon::Fifo::size()
{
    return m_size;
}

void dcommon::Fifo::enqueue(dcommon::tEntrySP p_entry) throw (dcommon::QueueException)
{
    dcommon::node_double_t *node = new dcommon::node_double_t(p_entry, m_tail, m_tail->previous);

    m_tail->previous->next = node;
    m_tail->previous = node;

    m_size++;
}

dcommon::tEntrySP dcommon::Fifo::dequeue()
{
    dcommon::node_double_t *temp = m_head->next;
    dcommon::tEntrySP result = temp->data;

    if (m_size == 0) {
        dcommon::entry_t *entry = NULL;
        return dcommon::tEntrySP(entry);
    }

    m_head->next = temp->next;
    temp->next->previous = m_head;

    temp->next = NULL;
    temp->previous = NULL;

    delete temp;
    m_size--;

    return result;
}

dcommon::node_double_t *dcommon::Fifo::delist()
{
    dcommon::entry_t *entry = NULL;
    dcommon::node_double_t *head = new dcommon::node_double_t(
        dcommon::tEntrySP(entry), m_head->next, NULL);
    dcommon::node_double_t *tail = new dcommon::node_double_t(
        dcommon::tEntrySP(entry), NULL, m_tail->previous);

    head->next->previous = head;
    tail->previous->next = tail;

    m_head->next = NULL;
    m_tail->previous = NULL;

//    delete m_head;
//    delete m_tail;

    // re-initialise the fifo data structure
    init();

    // return the current list
    return head;
}

void dcommon::Fifo::enlist(dcommon::node_double_t *p_list, long p_size)
{
    m_tail->previous->next = p_list;
    m_tail->previous = (p_list + (p_size - 1));

    m_size += p_size;
}
