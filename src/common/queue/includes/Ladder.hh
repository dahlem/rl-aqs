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
    int getNRung();
    int getThres();
    long getNBucket(int p_rung, int p_bucket);

    double getBucketwidth(int p_rung) throw (QueueException);
    double getRCur(int p_rung);
    double getRStart(int p_rung);
    
private:
    double bucketwidth(double p_max, double p_min, long p_n);
    double bucketwidth(int p_rung);
    int bucket(double p_TS, int p_rung);

    
    static const int THRES = 50;

    Fifo ***m_rungs;
    
    int m_NRung;
    int m_NBC;
    int m_BucketsFirstRung;

    double *m_bucketwidth;
    double *m_RCur;
    double *m_RStart;
};


#endif
