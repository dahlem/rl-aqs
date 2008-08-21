// Copyright (C) 2007,2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file Bottom.hh
 * Declaration of the bottom structure for the Ladder Queue.
 */
#ifndef BOTTOM_HH
#define BOTTOM_HH

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <boost/shared_ptr.hpp>

#include "Entry.hh"
#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"
namespace dcommon = des::common;



namespace des
{
    namespace common
    {


/**
 * This class represents the bottom structure of the <code>LadderQueue</code>.
 * All elements in the class are sorted and the last dequeued element time stamp
 * is kept to ensure that no events from the past are enqueued. </p>
 * For implementation details on the specific methods see the implementation
 * class.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie>Dominik Dahlem</a>
 */
class Bottom: public Queue, List
{
public:
    Bottom();
    ~Bottom();

#ifdef HAVE_LADDERSTATS
    void record();
#endif /* HAVE_LADDERSTATS */

    /**
     * @see Queue#enqueue(dcommon::tEntrySP) throw (QueueException)
     */
    void enqueue(dcommon::tEntrySP p_entry) throw (dcommon::QueueException);

    /**
     * @see Queue#dequeue()
     */
    dcommon::tEntrySP dequeue();

    /**
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(dcommon::node_double_t *p_list, long p_size);

    /**
     * @see List#delist()
     */
    dcommon::node_double_t * delist();

    /**
     * @return the maximum arrival timestamp of the events in the bottom.
     */
    double getMaxTS();

    /**
     * @return the minimum arrival timestamp of the events in the bottom.
     */
    double getMinTS();

    /**
     * @return the size of the event queue in the bottom
     */
    const long size();

private:
    /**
     * Initialise the bottom structure
     */
    void init();

    long m_size;
    double m_lastEvent;

    dcommon::node_double_t *m_head;
    dcommon::node_double_t *m_tail;
};


/** @typedef tBottomSP
 * a type defintion of a shared pointer of a bottom structure
 */
typedef boost::shared_ptr <Bottom> tBottomSP;


    }
}

#endif
