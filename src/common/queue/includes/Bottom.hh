#ifndef BOTTOM_HH
#define BOTTOM_HH


#include "Queue.hh"
#include "List.hh"



class Bottom: public Queue, List
{
public:
    Bottom();
    ~Bottom();

    void enqueue(entry_t *const p_entry);
    entry_t *const dequeue();
    void enlist(node_double_t *p_list, long p_size);
    node_double_t * delist();

    const long size();

private:
    void init();
    
    long m_size;
    node_double_t *m_head;
    node_double_t *m_tail;
};


#endif
