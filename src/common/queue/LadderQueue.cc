#include <cmath>

#include "LadderQueue.hh"
//#include "Ladder.hh"
//#include "Bottom.hh"



struct rung_t 
{
    struct rung_t *next;    
};



LadderQueue::LadderQueue() 
{
    m_top = new Top();
    
    m_ladder.nBc = 0;
    m_ladder.nRungs = 5;
    m_ladder.maxRungs = 8;
    m_ladder.thres = 50;
}

LadderQueue::~LadderQueue() 
{
    free(m_top);
}

void LadderQueue::enqueue(entry_t *const p_entry)
{
    int rung_pos = 0;
    
    if (p_entry->arrival >= m_top->getTopStart()) {
        // insert at the tail of top
        return;
    }

    while ((p_entry->arrival < m_ladder.rCur[rung_pos])
           && (rung_pos <= m_ladder.nRungs)) {
        rung_pos++;
    }

    if (rung_pos <= m_ladder.nRungs) {
        //int bucket_k = (int) floor((p_entry->arrival - m_ladder.rStart[rung_pos])
        //                           / m_ladder.bucketwidth[rung_pos]);
        // insert into tail of rung x, bucket_k
        // m_ladder.nBucket[rung_pos, bucket_k] += 1;
    } else {
    }
}

entry_t *const LadderQueue::dequeue()
{
    return NULL;
}
