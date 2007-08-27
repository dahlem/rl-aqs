#ifndef FIFO_HH
#define FIFO_HH


#include "Queue.hh"
#include "List.hh"



class Fifo: public Queue
{
public:
    Fifo();
    ~Fifo();

    void enqueue(entry_t *const p_entry);
    entry_t *const dequeue();
    node_double_t *getList();

    const long size();

private:
    void init();

    long m_size;
    node_double_t *m_head;
    node_double_t *m_tail;
};


#endif
