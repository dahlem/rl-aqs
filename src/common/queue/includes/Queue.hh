#ifndef QUEUE_HH
#define QUEUE_HH


#include "Entry.hh"
#include "QueueException.hh"



class Queue
{
public:
    virtual ~Queue() = 0;

    virtual void enqueue(entry_t *const p_entry) throw (QueueException) = 0;
    virtual entry_t *const dequeue() = 0;
};


#endif
