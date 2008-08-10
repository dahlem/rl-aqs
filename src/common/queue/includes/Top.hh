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
 * Declaration of the top structure of the Ladder Queue.
 */

#ifndef TOP_HH
#define TOP_HH


#include "Entry.hh"
using des::common::entry_t;

#include "Fifo.hh"
using des::common::Fifo;

#include "List.hh"
using des::common::node_double_t;
using des::common::List;

#include "Queue.hh"
using des::common::Queue;

#include "QueueException.hh"
using des::common::QueueException;



namespace des
{
    namespace common
    {



/**
 * This class represents the top structure of the <code>LadderQueue</code>.
 * All elements inserted into this class are unsorted and maintain FIFO
 * characteristics. This class maintains a few variables (timestamp, number of
 * events).
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Top: public Queue, List
{
public:
    Top();
    ~Top();

    /**
     * @see Queue#enqueue(entry_t* const) throw (QueueException)
     */
    void enqueue(entry_t *const p_entry) throw (QueueException);

    /**
     * @see Queue#dequeue()
     */
    entry_t *const dequeue();

    /**
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(node_double_t *p_list, long p_size);

    /**
     * @see List#delist()
     */
    node_double_t * delist();

    /**
     * @return double the maximum arrival timestamp
     */
    double getMaxTS();

    /**
     * @return double the minimum arrival timestamp
     */
    double getMinTS();

    /**
     * @return double the starting timestamp
     */
    double getTopStart();

    /**
     * @return double the number of events
     */
    long getNTop();

    /**
     * Reset the internal variables.
     * @throws QueueException thrown, if the reset is not allowed (which is the
     *                        case if the queue has not been emptied before).
     */
    void reset() throw (QueueException);

private:
    /**
     * @param double the maximum arrival timestamp
     */
    void setMaxTS(double p_maxTS);

    /**
     * @param double the minimum arrival timestamp
     */
    void setMinTS(double p_minTS);

    double m_maxTS;
    double m_minTS;
    double m_topStart;
    Fifo *m_fifo;
};

    }
}

#endif
