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

/** @file LadderQueue.cc
 * Implementation of the Ladder Queue @ref{LadderQueue.hh}.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if HAVE_LADDERTIMING
# include <ctime>
# include <iostream>
# include <ostream>
# include <string>
# include <sys/time.h>

# include <boost/iostreams/stream.hpp>
namespace bio = boost::iostreams;

# include <boost/shared_ptr.hpp>
#endif

#include <cstddef>

#include "LadderQueue.hh"
namespace dcommon = des::common;



dcommon::LadderQueue::LadderQueue()
{
    m_top = dcommon::tTopSP(new dcommon::Top());
    m_ladder = dcommon::tLadderSP(new dcommon::Ladder());
    m_bottom = dcommon::tBottomSP(new dcommon::Bottom());

#ifdef HAVE_LADDERTIMING
    std::string enqueue = "./ladder-enqueue-timing.txt";
    std::string dequeue = "./ladder-dequeue-timing.txt";

    // create a buffer
    bufEn = tStrBufSP(new str_buf(enqueue.c_str()));
    bufDe = tStrBufSP(new str_buf(dequeue.c_str()));

    // create a output stream
    osEn = tOstreamSP(new std::ostream(bufEn.get()));
    osDe = tOstreamSP(new std::ostream(bufDe.get()));
    (*osEn.get()) << "Time"  << std::endl;
    (*osDe.get()) << "Time"  << std::endl;
#endif /* HAVE_LADDERTIMING */
}

dcommon::LadderQueue::~LadderQueue()
{
}


#ifdef HAVE_LADDERSTATS
void dcommon::LadderQueue::record()
{
    m_top->record();
    m_ladder->record();
    m_bottom->record();
}
#endif /* HAVE_LADDERSTATS */


void dcommon::LadderQueue::enqueue(dcommon::tEntrySP p_entry) throw (dcommon::QueueException)
{
#ifdef HAVE_LADDERTIMING
    struct timeval start, finish;
    double time_delta = 0.0;
    gettimeofday(&start, NULL);
#endif /* HAVE_LADDERTIMING */

    if (p_entry->arrival >= m_top->getTopStart()) {
        // insert at the tail of top
        m_top->enqueue(p_entry);
    } else {


        try {
            m_ladder->enqueue(p_entry);
        } catch (dcommon::QueueException qe) {
            m_bottom->enqueue(p_entry);

            if (m_bottom->size() > m_ladder->getThres()) {
                // check whether ladder is empty
                // if yes, get max and min TS values from bottom and enlist
                if (m_ladder->getNBucket() == 0) {
                    double max = m_bottom->getMaxTS();
                    double min = m_bottom->getMinTS();
                    long size = m_bottom->size();

                    dcommon::node_double_t *list = m_bottom->delist();
                    m_ladder->enlist(list->next, size, max, min);
                } else {
                    // otherwise, spawn a new rung in the ladder and copy the
                    // events from bottom to the ladder rung just created.
                    bool success = m_ladder->spawn(false);

                    if (success) {
                        long size = m_bottom->size();
                        dcommon::node_double_t *list = m_bottom->delist();

                        m_ladder->pushBack(list, size);
                    }
                }
            }
        }
    }

#ifdef HAVE_LADDERTIMING
    gettimeofday(&finish, NULL);

    time_delta = (finish.tv_sec - start.tv_sec)
        + 1e-6 * (finish.tv_usec - start.tv_usec);

    (*osEn.get()) << time_delta  << std::endl;
#endif /* HAVE_LADDERTIMING */
}

dcommon::tEntrySP dcommon::LadderQueue::dequeue()
{
#ifdef HAVE_LADDERTIMING
    struct timeval start, finish;
    double time_delta = 0.0;
    gettimeofday(&start, NULL);
#endif /* HAVE_LADDERTIMING */

    dcommon::tEntrySP entry;

    if (m_bottom->size() > 0) {
        // bottom serves the dequeue operation
        entry = m_bottom->dequeue();
    } else {
        // otherwise the ladder will transfer events to the bottom
        int size = m_ladder->getNBucket();

        if (size > 0) {
            // the ladder contains events to be transferred to bottom
            m_bottom->enlist(m_ladder->delist()->next, size);
            entry = m_bottom->dequeue();
        } else {
            // check whether the top structure has events
            size = m_top->getNTop();
            double max = m_top->getMaxTS();
            double min = m_top->getMinTS();

            if (size > 0) {
                // the top structure transfers to the ladder and the ladder
                // to the bottom structure
                m_ladder->enlist(m_top->delist()->next, size, max, min);
                size = m_ladder->getNBucket();
                m_bottom->enlist(m_ladder->delist()->next, size);
                entry = m_bottom->dequeue();
            }
        }
    }


#ifdef HAVE_LADDERTIMING
    gettimeofday(&finish, NULL);

    time_delta = (finish.tv_sec - start.tv_sec)
        + 1e-6 * (finish.tv_usec - start.tv_usec);

    (*osDe.get()) << time_delta  << std::endl;
#endif /* HAVE_LADDERTIMING */

    return entry;
}
