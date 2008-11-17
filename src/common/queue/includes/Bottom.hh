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

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
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
class Bottom: public Queue
{
public:
    Bottom();
    ~Bottom();

#ifdef HAVE_LADDERSTATS
    void record();
#endif /* HAVE_LADDERSTATS */

    /**
     * @see Queue#enqueue(dcommon::Entry) throw (QueueException)
     */
    const bool push(dcommon::Entry *p_entry) throw (dcommon::QueueException);

    /**
     * @see Queue#dequeue() throw (QueueException)
     */
    dcommon::Entry* front() throw (dcommon::QueueException);
    void pop_front() throw (dcommon::QueueException);

    void push(dcommon::EntryList* );
    dcommon::EntryList* const list();

    /**
     * @return the maximum arrival timestamp of the events in the bottom.
     */
    const double getMaxTS();

    /**
     * @return the minimum arrival timestamp of the events in the bottom.
     */
    const double getMinTS();

    /**
     * @return the size of the event queue in the bottom
     */
    const boost::uint32_t size();

private:
    double m_lastEvent;
    dcommon::EntryList *m_list;
};


/** @typedef tBottomSP
 * a type defintion of a shared pointer of a bottom structure
 */
typedef boost::shared_ptr <Bottom> tBottomSP;


    }
}

#endif
