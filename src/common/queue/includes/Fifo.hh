// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef FIFO_HH
#define FIFO_HH


#include "Entry.hh"
using des::common::entry_t;

#include "List.hh"
using des::common::List;
using des::common::node_double_t;

#include "Queue.hh"
using des::common::Queue;

#include "QueueException.hh"
using des::common::QueueException;



namespace des
{
    namespace common
    {
        

/**
 * This class implements a first in first out data structure.
 *
 * <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Fifo: public Queue, List
{
public:
    Fifo();
    ~Fifo();

    /**
     * @see Queue#enqueue(entry_t* const) throw (QueueException)
     */
    void enqueue(entry_t *const p_entry) throw (QueueException);

    /**
     * @see Queue#dequeue()
     */
    entry_t *const dequeue();

    /**
     * @see List#delist()
     */
    node_double_t *delist();

    /**
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(node_double_t *p_list, long p_size);

    /**
     * @return the size of the fifo
     */
    const long size();

private:
    /**
     * Initialise the fifo.
     */
    void init();

    long m_size;
    node_double_t *m_head;
    node_double_t *m_tail;
};

    }
}

#endif
