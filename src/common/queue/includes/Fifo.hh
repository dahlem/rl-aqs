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

/** @file Fifo.hh
 * Declaration of the FIFO list/queue
 */

#ifndef __FIFO_HH__
#define __FIFO_HH__


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
 * This class implements a first in first out data structure.
 *
 * <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Fifo: public dcommon::Queue, dcommon::List
{
public:
    Fifo();
    ~Fifo();

    /**
     * @see Queue#enqueue(entry_t* const) throw (QueueException)
     */
    void enqueue(dcommon::tEntrySP p_entry) throw (dcommon::QueueException);

    /**
     * @see Queue#dequeue()
     */
    dcommon::tEntrySP dequeue();

    /**
     * @see List#delist()
     */
    dcommon::node_double_t *delist();

    /**
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(dcommon::node_double_t *p_list, long p_size);

    /**
     * @return the size of the fifo
     */
    const long size();

private:
    /**
     * Initialise the fifo.
     */
    void init();

    long m_size;
    dcommon::node_double_t *m_head;
    dcommon::node_double_t *m_tail;
};

    }
}

#endif
