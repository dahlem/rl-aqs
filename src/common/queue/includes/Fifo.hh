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
