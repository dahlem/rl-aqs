#ifndef BOTTOM_HH
#define BOTTOM_HH


#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"



class Bottom: public Queue, List
{
public:
    Bottom();
    ~Bottom();

    void enqueue(entry_t *const p_entry) throw (QueueException);
    entry_t *const dequeue();
    void enlist(node_double_t *p_list, long p_size);
    node_double_t * delist();
    double getMaxTS();
    double getMinTS();

    const long size();

private:
    void init();
    
    long m_size;
    double m_lastEvent;

    node_double_t *m_head;
    node_double_t *m_tail;
};


#endif
