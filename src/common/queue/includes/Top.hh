#ifndef TOP_HH
#define TOP_HH


#include "Fifo.hh"
#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"


class Top: public Queue, List
{
public:
    Top();
    ~Top();

    void enqueue(entry_t *const p_entry) throw (QueueException);
    entry_t *const dequeue();
    node_double_t *delist();
    void enlist(node_double_t *p_list, long p_size);

    double getMaxTS();
    double getMinTS();
    double getTopStart();
    long getNTop();
    void reset() throw (QueueException);

private:
    void setMaxTS(double p_maxTS);
    void setMinTS(double p_minTS);
    
    double m_maxTS;
    double m_minTS;
    double m_topStart;
    Fifo *m_fifo;
};


#endif
