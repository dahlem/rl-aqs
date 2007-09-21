// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LADDERQUEUE_HH
#define LADDERQUEUE_HH


#include "Bottom.hh"
#include "Ladder.hh"
#include "QueueException.hh"
#include "Top.hh"



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


#endif
