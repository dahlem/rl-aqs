// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef QUEUE_HH
#define QUEUE_HH


#include "Entry.hh"
#include "QueueException.hh"



/**
 * This class specifies the contract of a queue.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie>Dominik Dahlem</a>
 */
class Queue
{
public:
    virtual ~Queue() = 0;

    /**
     * @param entry_t* the entry structure to be enqueued into a queue.
     * @throws QueueException throw, if the queue cannot enqueue the entry.
     */
    virtual void enqueue(entry_t *const p_entry) throw (QueueException) = 0;

    /**
     * @return returns the next entry in a queue.
     */
    virtual entry_t *const dequeue() = 0;
};


#endif
