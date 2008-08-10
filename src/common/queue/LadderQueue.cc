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

/** @file LadderQueue.cc
 * Implementation of the Ladder Queue @ref{LadderQueue.hh}.
 */

#include <cstddef>

#include "LadderQueue.hh"
using des::common::LadderQueue;



LadderQueue::LadderQueue()
{
    m_top = new Top();
    m_ladder = new Ladder();
    m_bottom = new Bottom();
}

LadderQueue::~LadderQueue()
{
    delete m_top;
    delete m_ladder;
    delete m_bottom;
}

void LadderQueue::enqueue(entry_t *const p_entry) throw (QueueException)
{
    if (p_entry->arrival >= m_top->getTopStart()) {
        // insert at the tail of top
        m_top->enqueue(p_entry);
        return;
    }

    try {
        m_ladder->enqueue(p_entry);
    } catch (QueueException qe) {
        m_bottom->enqueue(p_entry);

        if (m_bottom->size() > m_ladder->getThres()) {
            // check whether ladder is empty
            // if yes, get max and min TS values from bottom and enlist
            if (m_ladder->getNBucket() == 0) {
                double max = m_bottom->getMaxTS();
                double min = m_bottom->getMinTS();
                long size = m_bottom->size();

                node_double_t *list = m_bottom->delist();
                m_ladder->enlist(list->next, size, max, min);
            } else {
                // otherwise, spawn a new rung in the ladder and copy the
                // events from bottom to the ladder rung just created.
                bool success = m_ladder->spawn(false);

                if (success) {
                    long size = m_bottom->size();
                    node_double_t *list = m_bottom->delist();

                    m_ladder->pushBack(list, size);
                }
            }
        }
    }
}

entry_t *const LadderQueue::dequeue()
{
    if (m_bottom->size() > 0) {
        // bottom serves the dequeue operation
        return m_bottom->dequeue();
    } else {
        // otherwise the ladder will transfer events to the bottom
        int size = m_ladder->getNBucket();

        if (size > 0) {
            // the ladder contains events to be transferred to bottom
            m_bottom->enlist(m_ladder->delist()->next, size);
            return m_bottom->dequeue();
        } else {
            // check whether the top structure has events
            size = m_top->getNTop();
            double max = m_top->getMaxTS();
            double min = m_top->getMinTS();

            if (size > 0) {
                // the top structure transfers to the ladder and the ladder
                // to the bottom structure
                m_ladder->enlist(m_top->delist()->next, size, max, min);
                size = m_ladder->getNBucket();
                m_bottom->enlist(m_ladder->delist()->next, size);
                return m_bottom->dequeue();
            }
        }
    }

    return NULL;
}
