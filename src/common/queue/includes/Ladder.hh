#ifndef LADDER_HH
#define LADDER_HH


#include "Fifo.hh"
#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"



class Ladder: public Queue
{
public:
    Ladder();
    ~Ladder();

    void enqueue(entry_t *const p_entry) throw (QueueException);
    entry_t *const dequeue();

    void enlist(node_double_t *p_list, long p_size);
    node_double_t *delist();

    int getNBC();
    int getNBucket(int p_bucket, int p_rung);
    int getNRung();
    int getThres();

    double getBucketwidth(int p_rung);
    double getRCur(int p_rung);
    double getRStart(int p_rung);

private:
    Fifo **m_rungs;
    
    int m_NRung;
    int m_NBC;
    int m_Thres;

    double *m_bucketwidth;
    int **m_NBucket;
    double *m_RCur;
    double *m_RStart;
};


#endif
