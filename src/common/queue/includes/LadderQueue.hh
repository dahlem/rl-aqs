#ifndef LADDERQUEUE_HH
#define LADDERQUEUE_HH


#include "Queue.hh"
#include "Top.hh"



struct ladder_t
{
    double *bucketwidth;
    int nBc;
    int **nBucket;
    int nRungs;
    int maxRungs;
    double *rCur;
    double *rStart;
    int thres;
};



class LadderQueue: public Queue
{
public:
    LadderQueue();
    ~LadderQueue();

    void enqueue(entry_t *const p_entry);
    entry_t *const dequeue();

private:
    Top *m_top;
    ladder_t m_ladder;
};


#endif
