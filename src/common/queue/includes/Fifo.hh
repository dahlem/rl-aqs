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


#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"



class Fifo: public Queue, List
{
public:
    Fifo();
    ~Fifo();

    void enqueue(entry_t *const p_entry) throw (QueueException);
    entry_t *const dequeue();
    node_double_t *delist();
    void enlist(node_double_t *p_list, long p_size);

    const long size();

    node_double_t *m_head;
    node_double_t *m_tail;
private:
    void init();

    long m_size;
};


#endif
