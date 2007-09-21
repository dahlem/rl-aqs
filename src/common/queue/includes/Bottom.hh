// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef BOTTOM_HH
#define BOTTOM_HH


#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"



/**
 * This class represents the bottom structure of the <code>LadderQueue</code>.
 * All elements in the class are sorted and the last dequeued element time stamp
 * is kept to ensure that no events from the past are enqueued. </p>
 * For implementation details on the specific methods see the implementation
 * class.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie>Dominik Dahlem</a>
 */
class Bottom: public Queue, List
{
public:
    Bottom();
    ~Bottom();

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
     * @return the maximum arrival timestamp of the events in the bottom.
     */ 
    double getMaxTS();

    /**
     * @return the minimum arrival timestamp of the events in the bottom.
     */
    double getMinTS();

    /**
     * @return the size of the event queue in the bottom
     */
    const long size();

private:
    /**
     * Initialise the bottom structure
     */
    void init();
    
    long m_size;
    double m_lastEvent;

    node_double_t *m_head;
    node_double_t *m_tail;
};


#endif
