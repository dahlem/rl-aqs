// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef TOP_HH
#define TOP_HH


#include "Entry.hh"
using des::common::entry_t;

#include "Fifo.hh"
using des::common::Fifo;

#include "List.hh"
using des::common::node_double_t;
using des::common::List;

#include "Queue.hh"
using des::common::Queue;

#include "QueueException.hh"
using des::common::QueueException;



namespace des
{
    namespace common
    {
        


/**
 * This class represents the top structure of the <code>LadderQueue</code>.
 * All elements inserted into this class are unsorted and maintain FIFO
 * characteristics. This class maintains a few variables (timestamp, number of
 * events).
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Top: public Queue, List
{
public:
    Top();
    ~Top();

    /**
     * @see Queue#enqueue(entry_t* const) throw (QueueException)
     */
    void enqueue(entry_t *const p_entry) throw (QueueException);

    /**
     * @see Queue#dequeue()
     */
    entry_t *const dequeue();

    /**
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(node_double_t *p_list, long p_size);

    /**
     * @see List#delist()
     */
    node_double_t * delist();

    /**
     * @return double the maximum arrival timestamp
     */
    double getMaxTS();

    /**
     * @return double the minimum arrival timestamp
     */
    double getMinTS();

    /**
     * @return double the starting timestamp
     */
    double getTopStart();

    /**
     * @return double the number of events
     */
    long getNTop();

    /**
     * Reset the internal variables.
     * @throws QueueException thrown, if the reset is not allowed (which is the
     *                        case if the queue has not been emptied before).
     */
    void reset() throw (QueueException);

private:
    /**
     * @param double the maximum arrival timestamp
     */
    void setMaxTS(double p_maxTS);

    /**
     * @param double the minimum arrival timestamp
     */
    void setMinTS(double p_minTS);
    
    double m_maxTS;
    double m_minTS;
    double m_topStart;
    Fifo *m_fifo;
};

    }
}

#endif
