#ifndef LADDERQUEUE_HH
#define LADDERQUEUE_HH


#include "Top.hh"
#include "Ladder.hh"
#include "Bottom.hh"



class LadderQueue: public Queue
{
public:
    LadderQueue();
    ~LadderQueue();

    void enqueue(entry_t *const p_entry);
    entry_t *const dequeue();

private:
    Top *m_top;
    Ladder *m_ladder;
    Bottom *m_bottom;
};


#endif
