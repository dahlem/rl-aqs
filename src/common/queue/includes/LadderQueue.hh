// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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
