// Copyright (C) 2007,2008,2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file Top.hh
 * Declaration of the top structure of the Ladder Queue.
 */
#ifndef __TOP_HH__
#define __TOP_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#ifdef HAVE_LADDERSTATS
# include <boost/shared_ptr.hpp>
#endif /* HAVE_LADDERSTATS */

#include <boost/cstdint.hpp>
#include <boost/scoped_ptr.hpp>

#include "Entry.hh"
#include "Queue.hh"
#include "QueueException.hh"
namespace dcommon = des::common;



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
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class Top: public dcommon::Queue
{
public:
    Top();
    ~Top();

#ifdef HAVE_LADDERSTATS
    void record();
#endif /* HAVE_LADDERSTATS */

    bool push(dcommon::Entry *p_entry) throw (dcommon::QueueException);
    void push(dcommon::EntryList* );
    dcommon::EntryList* delist();

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
    boost::uint32_t getNTop();

    /**
     * Reset the internal variables.
     * @throws QueueException thrown, if the reset is not allowed (which is the
     *                        case if the queue has not been emptied before).
     */
    void reset() throw (dcommon::QueueException);

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

    dcommon::EntryList *m_fifo;

};


/** @typedef tTopSP
 * a type defintion of a shared pointer of a top structure
 */
typedef boost::scoped_ptr <Top> tTopSP;


    }
}

#endif
