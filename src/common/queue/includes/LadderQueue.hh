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

/** @file LadderQueue.hh
 * Declaration of the Ladder Queue.
 */

#ifndef LADDERQUEUE_HH
#define LADDERQUEUE_HH


#include "Bottom.hh"
using des::common::Bottom;

#include "Entry.hh"
using des::common::entry_t;

#include "Ladder.hh"
using des::common::Ladder;

#include "Queue.hh"
using des::common::Queue;

#include "QueueException.hh"
using des::common::QueueException;

#include "Top.hh"
using des::common::Top;


namespace des
{
    namespace common
    {


/**
 * This class is based on the journal publication:
 * <ul>
 *   <li>Ladder Queue: An O(1) Priority Queue Structure for Large-Scale
 *       Discrete Event Simulation</br>
 *       Wai Teng Tang, Rick Siow Mong Goh, and Ian Li-Jin Thng</br>
 *       ACM Transactions on Modeling and Computer Simulation, Vol. 15, No. 3,
 *       July 2005, Pages 175-204</li>
 * </ul>
 *
 * Some internal details are adapted and improved upon. The top, ladder, and
 * bottom structures pass partial lists around, which violates data encapsulation,
 * but provides better performance, because list elements don't have to be
 * free'd and created several times.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class LadderQueue: public Queue
{
public:
    LadderQueue();
    ~LadderQueue();

    /**
     * @see Queue#enqueue(entry_t* const) throw (QueueException)
     */
    void enqueue(entry_t *const p_entry) throw (QueueException);

    /**
     * @see Queue#dequeue()
     */
    entry_t *const dequeue();

private:
    Top *m_top;
    Ladder *m_ladder;
    Bottom *m_bottom;
};

    }
}

#endif
