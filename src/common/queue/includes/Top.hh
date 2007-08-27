#ifndef TOP_HH
#define TOP_HH


#include "Queue.hh"
#include "Fifo.hh"
#include "QueueException.hh"


class Top: public Queue
{
public:
    Top();
    ~Top();

    void enqueue(entry_t *const p_entry);
    entry_t *const dequeue();

    double getMaxTS();
    double getMinTS();
    double getTopStart();
    long getNTop();
    void reset() throw (QueueException);
    node_double_t *delist();

private:
    void setMaxTS(double p_maxTS);
    void setMinTS(double p_minTS);
    
    double m_maxTS;
    double m_minTS;
    double m_topStart;
    Fifo *m_fifo;
};


#endif
