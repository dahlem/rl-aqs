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
using des::common::Fifo;



Fifo::Fifo()
{
    m_head = new node_double_t(NULL, NULL, NULL);
    m_tail = new node_double_t(NULL, NULL, NULL);

    init();
}


Fifo::~Fifo()
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

void Fifo::init()
{
    m_head->next = m_tail;
    m_tail->previous = m_head;
    m_size = 0;
}

const long Fifo::size()
{
    return m_size;
}

void Fifo::enqueue(entry_t *const p_entry) throw (QueueException)
{
    node_double_t *node = new node_double_t(p_entry, m_tail, m_tail->previous);

    m_tail->previous->next = node;
    m_tail->previous = node;

    m_size++;
}

entry_t *const Fifo::dequeue()
{
    node_double_t *temp = m_head->next;
    entry_t *const result = temp->data;

    if (m_size == 0) {
        return NULL;
    }

    m_head->next = temp->next;
    temp->next->previous = m_head;

    temp->next = NULL;
    temp->previous = NULL;

    delete temp;
    m_size--;

    return result;
}

node_double_t *Fifo::delist()
{
    node_double_t *head = new node_double_t(NULL, m_head->next, NULL);
    node_double_t *tail = new node_double_t(NULL, NULL, m_tail->previous);

    head->next->previous = head;
    tail->previous->next = tail;

    // re-initialise the fifo data structure
    init();

    // return the current list
    return head;
}

void Fifo::enlist(node_double_t *p_list, long p_size)
{
    m_tail->previous->next = p_list;
    m_tail->previous = (p_list + (p_size - 1));

    m_size += p_size;
}
