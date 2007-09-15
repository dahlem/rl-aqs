#ifndef LADDERQUEUE_HH
#define LADDERQUEUE_HH


#include "Bottom.hh"
#include "Ladder.hh"
#include "QueueException.hh"
#include "Top.hh"



class LadderQueue: public Queue
{
public:
    LadderQueue();
    ~LadderQueue();

    void enqueue(entry_t *const p_entry) throw (QueueException);
    entry_t *const dequeue();

private:
    Top *m_top;
    Ladder *m_ladder;
    Bottom *m_bottom;
};


#endif
