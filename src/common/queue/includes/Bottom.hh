#ifndef BOTTOM_HH
#define BOTTOM_HH


#include "Queue.hh"
#include "List.hh"



class Bottom: public Queue
{
public:
    Bottom();
    ~Bottom();

    void enqueue(entry_t *const p_entry);
    void enlist(node_double_t *p_list, long p_size);
    entry_t *const dequeue();

    const long size();

private:
    long m_size;
    node_double_t *m_head;
    node_double_t *m_tail;
};


#endif
