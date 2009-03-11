// Copyright (C) 2007,2008,2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file Ladder.cc
 * Implementation of the ladder structure @ref{Ladder.hh} of the Ladder Queue.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if !defined(NDEBUG) || !defined(NDEBUG_QUEUE)
# include <iostream>
#endif /* NDEBUG_EVENTS */
# include <iostream>

#ifdef HAVE_LADDERSTATS
# include <iostream>
# include <ostream>
# include <string>

# include <boost/iostreams/stream.hpp>
namespace bio = boost::iostreams;

# include <boost/shared_ptr.hpp>
#endif /* HAVE_LADDERSTATS */

#include <iomanip>
#include <cmath>
#include <cstddef>
#include <limits>

#include "Ladder.hh"
namespace dcommon = des::common;



dcommon::Ladder::Ladder(boost::uint32_t p_thres)
{
    m_Thres = p_thres;
    init();
}

dcommon::Ladder::Ladder()
{
    m_Thres = DEFAULT_THRES;
    init();
}

void dcommon::Ladder::init()
{
    m_NRung = 5;
    m_lowestRung = 0;
    m_NBC = 0;
    m_BucketsFirstRung = m_Thres;
    epochDelisted = false;

    m_events = dcommon::tIntSA(new boost::uint32_t[m_NRung]);
    m_currentBucket = dcommon::tIntSA(new boost::uint32_t[m_NRung]);
    m_bucketwidth = dcommon::tDoubleSA(new double[m_NRung]);
    m_RCur = dcommon::tDoubleSA(new double[m_NRung]);
    m_RStart = dcommon::tDoubleSA(new double[m_NRung]);

    // initialise the allocated memory
    memset(m_events.get(), 0, sizeof(boost::uint32_t) * m_NRung);
    memset(m_currentBucket.get(), 0, sizeof(boost::uint32_t) * m_NRung);
    memset(m_bucketwidth.get(), 0, sizeof(double) * m_NRung);

    m_rungs = dcommon::EntryListSM(new dcommon::EntryListSA[m_NRung]);

    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        m_rungs[i] = dcommon::EntryListSA(new dcommon::EntryList[m_Thres]);
        m_RCur[i] = std::numeric_limits<double>::max();
        m_RStart[i] = std::numeric_limits<double>::max();
    }

#ifdef HAVE_LADDERSTATS
    events_in = 0;
    events_out = 0;

    std::string ladderfile = "./ladder-stats.txt";

    // create a buffer
    buf = tStrBufSP(new str_buf(ladderfile.c_str()));

    // create a output stream
    os = tOstreamSP(new std::ostream(buf.get()));
    (*os.get()) << "EventsIn,EventsOut,NumEvents,NumRungs"  << std::endl;
#endif /* HAVE_LADDERSTATS */
}


dcommon::Ladder::~Ladder()
{
    for (boost::uint32_t i = 0; i < m_BucketsFirstRung; ++i) {
        dcommon::EntryList *list =
            reinterpret_cast<dcommon::EntryList*>(
                &(m_rungs[0][i]));
        list->erase_and_dispose(list->begin(), list->end(),
                               dcommon::delete_disposer());
    }

    for (boost::uint32_t i = 1; i < m_NRung; ++i) {
        for (boost::uint32_t j = 0; j < m_Thres; ++j) {
            dcommon::EntryList *list =
                reinterpret_cast<dcommon::EntryList*>(
                    &(m_rungs[i][j]));
            list->erase_and_dispose(list->begin(), list->end(),
                                    dcommon::delete_disposer());
        }
    }
}


#ifndef NDEBUG
void dcommon::Ladder::printEvents()
{
    std::cout << "Ladder -- size per rung: " << std::endl;
    for (boost::uint16_t i = 0; i < m_NRung; ++i) {
        std::cout << i << ": " << m_events[i] << std::endl;
    }
}
#endif /* NDEBUG */


#ifdef HAVE_LADDERSTATS
void dcommon::Ladder::record()
{
    (*os.get()) << events_in << "," << events_out << "," << getNBC() << getNRung() << std::endl;

    // reset the stat fields
    events_in = 0;
    events_out = 0;
}
#endif /* HAVE_LADDERSTATS */


boost::uint32_t dcommon::Ladder::getNBC()
{
    return m_NBC;
}

boost::uint32_t dcommon::Ladder::getThres()
{
    return m_Thres;
}

double dcommon::Ladder::bucketwidth(double p_max, double p_min, boost::uint32_t p_n)
{
    if (p_max == p_min) {
        return 1.0;
    } else {
        return (p_max - p_min) / p_n;
    }
}

boost::uint32_t dcommon::Ladder::bucket(double p_TS, boost::uint32_t p_rung)
{
    double diff = (p_TS - m_RStart[p_rung]);
    double result = diff / m_bucketwidth[p_rung];
    boost::uint32_t retVal = (boost::uint32_t) floor(result);

    return retVal;
}

void dcommon::Ladder::updateNEvents(boost::uint32_t p_rung, boost::int32_t size)
{
    m_events[p_rung] += size;
    m_NBC += size;

#ifdef HAVE_LADDERSTATS
    events_in += size;

    if (size < 0) {
        events_out += (-size);
    }
#endif /* HAVE_LADDERSTATS */
}


const bool dcommon::Ladder::push(dcommon::Entry *p_entry) throw (dcommon::QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- Push event: " << const_cast <const dcommon::Entry&> (*p_entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    // cannot enqueue, if the internal structure has not been initialised
    // by an epoch
    if (getNBC() == 0) {
        throw dcommon::QueueException(dcommon::QueueException::NO_EPOCH_INIT);
    }

    boost::uint32_t nRungs = 0;
    double ts_arrival = p_entry->getArrival();

    // find the rung
    while (true) {
        if ((ts_arrival < m_RCur[nRungs]) && (nRungs < m_NRung)) {
            nRungs++;
        } else {
            break;
        }
    }

    // found
    if (nRungs <= m_lowestRung) {
        // insert into tail of rung x, bucket k
#ifndef NDEBUG_QUEUE
        std::cout << "Ladder -- Rung: " << nRungs << ", bucket: "
                  << bucket(ts_arrival, nRungs);
#endif /* NDEBUG_EVENTS */

        boost::uint32_t l_bucket = bucket(ts_arrival, nRungs);
        if (l_bucket <= m_currentBucket[nRungs]) {
//             throw dcommon::QueueException(dcommon::QueueException::BUCKET_EQUALS_DEQUEUEBUCKET);
        }

        m_rungs[nRungs][l_bucket].push_back(*p_entry);

#ifndef NDEBUG_QUEUE
        std::cout << ", size: " << m_rungs[nRungs][l_bucket].size()
                  << std::endl;
#endif /* NDEBUG_EVENTS */

        updateNEvents(nRungs, 1);
    } else {
        throw dcommon::QueueException(dcommon::QueueException::RUNG_NOT_FOUND);
    }

    return true;
}

dcommon::Entry* dcommon::Ladder::front() throw (dcommon::QueueException)
{
    return NULL;
}

void dcommon::Ladder::pop_front() throw (dcommon::QueueException)
{
}


void dcommon::Ladder::push(boost::uint32_t p_rung, dcommon::EntryList *p_list)
{
    dcommon::Entry *entry = NULL;

    // transfer one event at a time
    while (!p_list->empty()) {
        entry = reinterpret_cast<dcommon::Entry*>(&p_list->front());
        boost::uint32_t l_bucket = bucket(entry->getArrival(), p_rung);
        p_list->pop_front();
        m_rungs[p_rung][l_bucket].push_back(*entry);

        updateNEvents(p_rung, 1);
    }
}


void dcommon::Ladder::pushBack(dcommon::EntryList *p_list)
    throw (dcommon::QueueException)
{
    std::cout << "push back: " << p_list->size() << std::endl;

    bool spawned = false;

    spawned = spawn(false);
    if (!spawned) {
        throw dcommon::QueueException(dcommon::QueueException::RUNG_OUT_OF_BOUNDS);
    }

    push(m_lowestRung, p_list);
}


void dcommon::Ladder::push(dcommon::EntryList *p_list, double p_maxTS, double p_minTS)
    throw (dcommon::QueueException)
{
    if (getNBC() != 0) {
        throw dcommon::QueueException(dcommon::QueueException::EPOCH_EXISTS);
    }

    // nothing to do
    if (p_list->size() == 0) {
        return;
    }

    // init variables
    m_lowestRung = 0;
    m_bucketwidth[0] = bucketwidth(p_maxTS, p_minTS, p_list->size());
    m_RStart[0] = p_minTS;
    m_RCur[0] = p_minTS; // + m_bucketwidth[0]; // different in paper

    // reset the current dequeue buckets
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        m_currentBucket[i] = 0;
    }

    boost::uint32_t buckets = static_cast<boost::uint32_t>(
        (p_maxTS - p_minTS) / m_bucketwidth[0]);

    if (buckets >= m_BucketsFirstRung) {
        resizeFirstRung(buckets);
    }

    // transfer events
    push(m_lowestRung, p_list);
    epochDelisted = false;
}

dcommon::EntryList* const dcommon::Ladder::delist() throw (dcommon::QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- Delist" << std::endl;
#endif /* NDEBUG_EVENTS */

    // advance the dequeue bucket
    advanceDequeueBucket(true);

    // get the nodes of the list
    dcommon::EntryList *temp =
        reinterpret_cast<dcommon::EntryList*>(
            &(m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]));
    boost::uint32_t size = temp->size();

    updateNEvents(m_lowestRung, -size);

#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- Delist: " << m_lowestRung << ", bucket: "
              << m_currentBucket[m_lowestRung] << ", size: " << temp->size()
              << std::endl;
#endif /* NDEBUG_EVENTS */

    epochDelisted = true;

    // once delisted we increment rcur to invalidate the previous bucket
    //m_currentBucket[m_lowestRung]++; // do we need this here?
    //m_RCur[m_lowestRung] += m_bucketwidth[m_lowestRung];
    if ((m_lowestRung > 0) && (m_events[m_lowestRung] == 0)) {
        m_RCur[m_lowestRung] = std::numeric_limits<double>::max();
        m_RStart[m_lowestRung] = std::numeric_limits<double>::max();
        m_currentBucket[m_lowestRung] = 0;
        m_lowestRung--;
//     } else {
//         m_RCur[m_lowestRung] += m_bucketwidth[m_lowestRung];
//         m_currentBucket[m_lowestRung]++;
    }

    return temp;
}

void dcommon::Ladder::resizeFirstRung(boost::uint32_t p_base)
{
    // resize the rung size
    m_BucketsFirstRung = 2 * p_base;
    m_rungs[0] = dcommon::EntryListSA(new dcommon::EntryList[m_BucketsFirstRung]);
}

void dcommon::Ladder::advanceDequeueBucket(bool p_spawn) throw (dcommon::QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- advance dequeue bucket" << std::endl;
#endif /* NDEBUG_EVENTS */

    boost::uint32_t elements = 0;

    // skip empty tail buckets of the current and lower rungs
    while (true) {
        if ((m_lowestRung > 0) && (m_events[m_lowestRung] == 0)) {
            m_RCur[m_lowestRung] = std::numeric_limits<double>::max();
            m_RStart[m_lowestRung] = std::numeric_limits<double>::max();
            m_currentBucket[m_lowestRung] = 0;
            m_lowestRung--;
        } else {
            break;
        }
    }

    // find next non-empty bucket from lowest rung
    while (true) {
#ifndef NDEBUG_QUEUE
        std::cout << "Ladder -- advance on rung " << m_lowestRung
                  << " from bucket " << m_currentBucket[m_lowestRung]
                  << ", num events: " << m_events[m_lowestRung] << std::endl;
#endif /* NDEBUG_EVENTS */

        // how many elements are in this bucket?
        elements = m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]].size();

#ifndef NDEBUG_QUEUE
        std::cout << "Ladder -- rung " << m_lowestRung
                  << ", num events " << elements << " in bucket "
                  << m_currentBucket[m_lowestRung] << std::endl;
#endif /* NDEBUG_EVENTS */

        // advance to next bucket
        if (elements == 0) {
            m_currentBucket[m_lowestRung]++;
            m_RCur[m_lowestRung] += m_bucketwidth[m_lowestRung];
        } else {
            break;
        }
    }

    if (p_spawn && (elements > m_Thres)) {
        bool spawned = spawn(true);
        advanceDequeueBucket(spawned);
    }

#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- advance dequeue bucket finished" << std::endl;
#endif /* NDEBUG_EVENTS */
}

bool dcommon::Ladder::canAdvance()
{
    bool advance = true;

    // leave, if the current bucket cannot be advanced further
    // two cases: (1) we are on the first rung; (2) we are between 0-m_NRung
    if (m_lowestRung == 0) {
        if (m_currentBucket[m_lowestRung] == (m_BucketsFirstRung - 1)) {
            advance = false;
        }
   } else if (m_lowestRung < m_NRung) {
        if (m_currentBucket[m_lowestRung] == (m_Thres - 1)) {
           advance = false;
       }
    }

    return advance;
}

bool dcommon::Ladder::spawn(bool p_doEnlist)
{
    bool result = false;

    // spawn, iff
    // (1) lowest rung < MAX_RUNGS - 1
    if ((m_lowestRung < (MAX_RUNGS - 1))) {
        m_lowestRung++;

        // create new rung, iff
        // (1) m_NRung < MAX_RUNGS
        // (2) m_lowestRung >= m_NRung
        if ((m_lowestRung >= m_NRung) && (m_NRung < MAX_RUNGS)) {
            createRung();
        }

//         if (p_doEnlist) {
//             std::cout << std::setprecision(14)
//                       << (m_lowestRung - 1) << " : "
//                       << m_currentBucket[m_lowestRung - 1] << " : "
//                       << m_events[m_lowestRung - 1]
//                       << ", rstart: " << m_RStart[m_lowestRung - 1]
//                       << ", rcur: " << m_RCur[m_lowestRung - 1]
//                       << ", bucketwidth: " << m_bucketwidth[m_lowestRung - 1]
//                       << std::endl;
//         }

        // these formulas are different from the ones presented in the TOMACS
        // journal on the Ladder Queue by Tang, et. al.
        m_bucketwidth[m_lowestRung] =
            m_bucketwidth[m_lowestRung - 1] / (m_Thres  - 1);
        m_RStart[m_lowestRung] =
            m_RCur[m_lowestRung - 1];// - m_bucketwidth[m_lowestRung - 1];
        m_RCur[m_lowestRung] =
            m_RStart[m_lowestRung];// + m_bucketwidth[m_lowestRung];
        m_RCur[m_lowestRung - 1] += m_bucketwidth[m_lowestRung - 1];

        // we need to set the current dequeue bucket of the spawned rung to zero
        m_currentBucket[m_lowestRung] = 0;


        if (p_doEnlist) {
            // copy the elements from the previous bucket to the next rung
            dcommon::EntryList *list =
                reinterpret_cast<dcommon::EntryList*>(
                    &(m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]));

//             std::cout << std::setprecision(14) << list->size()
//                       << ", rstart: " << m_RStart[m_lowestRung]
//                       << ", rcur: " << m_RCur[m_lowestRung]
//                       << ", bucketwidth: " << m_bucketwidth[m_lowestRung]
//                       << std::endl;

//             dcommon::EntryList::iterator it, it_end;
//             it = list->begin();
//             it_end = list->end();

//             for (; it != it_end; ++it) {
//                 std::cout << it->getArrival() << std::endl;
//             }

            updateNEvents(m_lowestRung - 1, -list->size());
            push(m_lowestRung, list);
        }

        m_currentBucket[m_lowestRung - 1]++;
        result = true;
    }

    return result;
}

void dcommon::Ladder::createRung()
{
    m_NRung++;

    // create a new rung
    dcommon::EntryListSM rungs = dcommon::EntryListSM(new dcommon::EntryListSA[m_NRung]);

    // copy the old rungs over
    for (boost::uint32_t i = 0; i < (m_NRung - 1); ++i) {
        rungs[i] = m_rungs[i];
    }
    rungs[m_NRung - 1] = dcommon::EntryListSA(new dcommon::EntryList[m_Thres]);

    m_rungs = rungs;

    // resize the events data structure
    boost::uint32_t *events = new boost::uint32_t[m_NRung];
    memset(events, 0, sizeof(boost::uint32_t) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        events[i] = m_events[i];
    }

    m_events = dcommon::tIntSA(events);

    // resize the currentBucket data structure
    boost::uint32_t *currentBucket = new boost::uint32_t[m_NRung];
    memset(currentBucket, 0, sizeof(boost::uint32_t) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        currentBucket[i] = m_currentBucket[i];
    }

    m_currentBucket = dcommon::tIntSA(currentBucket);

    // resize the bucketwidth data structure
    double *bucketwidth = new double[m_NRung];
    memset(bucketwidth, 0, sizeof(double) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        bucketwidth[i] = m_bucketwidth[i];
    }

    m_bucketwidth = dcommon::tDoubleSA(bucketwidth);

    // resize the rcur data structure
    double *rcur = new double[m_NRung];
    memset(rcur, 0, sizeof(double) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        rcur[i] = m_RCur[i];
    }

    m_RCur = dcommon::tDoubleSA(rcur);

    // resize the rstart data structure
    double *rstart = new double[m_NRung];
    memset(rstart, 0, sizeof(double) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        rstart[i] = m_RStart[i];
    }

    m_RStart = dcommon::tDoubleSA(rstart);
}
