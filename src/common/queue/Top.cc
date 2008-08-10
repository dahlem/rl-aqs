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

#include <cstddef>
#include <cfloat>

#include "Top.hh"
using des::common::Top;



Top::Top()
    : m_maxTS(0.0), m_minTS(DBL_MAX), m_topStart(0.0)
{
    m_fifo = new Fifo();
}

Top::~Top()
{
    delete m_fifo;
}

double Top::getMaxTS()
{
    return m_maxTS;
}

double Top::getMinTS()
{
    return m_minTS;
}

double Top::getTopStart()
{
    return m_topStart;
}

long Top::getNTop()
{
    return m_fifo->size();
}

void Top::reset() throw (QueueException)
{
    if (getNTop() > 0) {
        throw QueueException(QueueException::BAD_RESET);
    } else {
        m_maxTS = 0.0;
        m_minTS = DBL_MAX;
    }
}

void Top::enqueue(entry_t *const p_entry) throw (QueueException)
{
    setMaxTS(p_entry->arrival);
    setMinTS(p_entry->arrival);

    m_fifo->enqueue(p_entry);
}

entry_t *const Top::dequeue()
{
    entry_t *result = m_fifo->dequeue();

    if (result != NULL) {
        m_topStart = result->arrival;
    }

    return result;
}

void Top::setMaxTS(double p_maxTS)
{
    if (p_maxTS > m_maxTS) {
        m_maxTS = p_maxTS;
    }
}

void Top::setMinTS(double p_minTS)
{
    if (p_minTS < m_minTS) {
        m_minTS = p_minTS;
    }
}

node_double_t *Top::delist()
{
    node_double_t *result = NULL;

    try {
        result = m_fifo->delist();
        m_topStart = m_maxTS;

        reset();
    } catch (QueueException &qe) {
        // cannot happen here
    }

    return result;
}

void Top::enlist(node_double_t *p_list, long p_size)
{
    m_fifo->enlist(p_list, p_size);
}
