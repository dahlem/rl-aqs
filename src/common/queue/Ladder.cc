// Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
# include <cassert>
# include <iomanip>
# include <iostream>
#endif /* NDEBUG && NDEBUG_EVENTS */

#ifdef HAVE_LADDERSTATS
# include <iostream>
# include <ostream>
# include <string>

# include <boost/iostreams/stream.hpp>
namespace bio = boost::iostreams;

# include <boost/shared_ptr.hpp>
#endif /* HAVE_LADDERSTATS */

#include <cmath>
#include <cstddef>
#include <cstring>
#include <limits>

#include <gsl/gsl_math.h>

#include "Ladder.hh"


namespace des
{
namespace common
{


Ladder::Ladder(boost::uint32_t p_thres)
    : m_Thres(p_thres), m_NRung(3), m_lowestRung(0), m_NBC(0),
      m_BucketsFirstRung(p_thres), m_events(new boost::uint32_t[m_NRung]),
      m_currentBucket(new boost::uint32_t[m_NRung]),
      m_buckets(new boost::uint32_t[m_NRung]), m_bucketwidth(new double[m_NRung]),
      m_RCur(new double[m_NRung]), m_RStart(new double[m_NRung])
{
    init();
}

Ladder::Ladder()
    : m_Thres(DEFAULT_THRES), m_NRung(3), m_lowestRung(0), m_NBC(0),
      m_BucketsFirstRung(DEFAULT_THRES), m_events(new boost::uint32_t[m_NRung]),
      m_currentBucket(new boost::uint32_t[m_NRung]),
      m_buckets(new boost::uint32_t[m_NRung]), m_bucketwidth(new double[m_NRung]),
      m_RCur(new double[m_NRung]), m_RStart(new double[m_NRung])
{
    init();
}

void Ladder::init()
{
    // initialise the allocated memory
    memset(m_events.get(), 0, sizeof(boost::uint32_t) * m_NRung);
    memset(m_currentBucket.get(), 0, sizeof(boost::uint32_t) * m_NRung);
    memset(m_buckets.get(), 0, sizeof(boost::uint32_t) * m_NRung);
    memset(m_bucketwidth.get(), 0, sizeof(double) * m_NRung);

    m_rungs = EntryListSM(new EntryListSA[m_NRung]);

    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        m_rungs[i] = EntryListSA(new EntryList[m_Thres]);
        m_RCur[i] = std::numeric_limits<double>::max();
        m_RStart[i] = std::numeric_limits<double>::max();

        if (i == 0) {
            m_buckets[0] = m_BucketsFirstRung;
        } else {
            m_buckets[i] = m_Thres;
        }
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


Ladder::~Ladder()
{
    for (boost::uint32_t i = 0; i < m_BucketsFirstRung; ++i) {
        EntryList *list =
            reinterpret_cast<EntryList*>(
                &(m_rungs[0][i]));
        list->erase_and_dispose(list->begin(), list->end(),
                               delete_disposer());
    }

    for (boost::uint32_t i = 1; i < m_NRung; ++i) {
        for (boost::uint32_t j = 0; j < m_Thres; ++j) {
            EntryList *list =
                reinterpret_cast<EntryList*>(
                    &(m_rungs[i][j]));
            list->erase_and_dispose(list->begin(), list->end(),
                                    delete_disposer());
        }
    }
}


#ifndef NDEBUG
void Ladder::printEvents()
{
    std::cout << "Ladder -- size per rung: " << std::endl;
    for (boost::uint16_t i = 0; i < m_NRung; ++i) {
        std::cout << i << ": " << m_events[i] << std::endl;
    }
}
#endif /* NDEBUG */


#ifdef HAVE_LADDERSTATS
void Ladder::record()
{
    (*os.get()) << events_in << "," << events_out << "," << getNBC() << getNRung() << std::endl;

    // reset the stat fields
    events_in = 0;
    events_out = 0;
}
#endif /* HAVE_LADDERSTATS */


boost::uint32_t Ladder::getNBC()
{
    return m_NBC;
}

boost::uint32_t Ladder::getThres()
{
    return m_Thres;
}

double Ladder::bucketwidth(double p_max, double p_min, boost::uint32_t p_n)
{
    if (p_max == p_min) {
        return 1.0;
    } else {
        return (p_max - p_min) / p_n;
    }
}

boost::uint32_t Ladder::bucket(double p_TS, boost::uint32_t p_rung)
{
    double diff = 0.0;

    // if the difference is too small we set it to zero
    diff = (gsl_fcmp(p_TS, m_RStart[p_rung], 1e-9) <= 0) ? (0.0) : (p_TS - m_RStart[p_rung]);

    double result = diff / m_bucketwidth[p_rung];
    boost::uint32_t retVal = (boost::uint32_t) floor(result);

    // since rstart allowed the enqueue event, and l_bucket is smaller than the
    // current bucket, we need to adjust the rounding error that had occurred
    if (retVal < m_currentBucket[p_rung]) {
        retVal++;
    }

#ifndef NDEBUG
    assert(retVal >= m_currentBucket[p_rung]);
    assert(retVal <= m_buckets[p_rung]);
#endif /* NDEBUG */

    // use interval iteration to find the correct bucket
//     if (retVal > m_buckets[p_rung]) {
//     }

    return retVal;
}

void Ladder::updateNEvents(boost::uint32_t p_rung, boost::int32_t size)
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


bool Ladder::push(Entry *p_entry) throw (QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- Push event: " << const_cast <const Entry&> (*p_entry) << std::endl;
#endif /* NDEBUG_QUEUE */

    // cannot enqueue, if the internal structure has not been initialised
    // by an epoch
    if (getNBC() == 0) {
        throw QueueException(QueueException::NO_EPOCH_INIT);
    }

    boost::uint32_t nRungs = 0;
    double ts_arrival = p_entry->getArrival();

    // check the timestamp against the rcur value of the lowest rung
    // if the rcur value of the lowest rung is larger than the TS then it doesn't fit anywhere
    // this condition is not mentioned in the tomacs journal article
    if (ts_arrival < m_RCur[m_lowestRung]) {
        throw QueueException(QueueException::RUNG_NOT_FOUND);
    }

    // find the rung
    // we know that at least the lowest rung can take the event
    // but we look more carefully now starting from rung 0
    while (true) {
        if ((ts_arrival < m_RCur[nRungs])
            && (nRungs < m_lowestRung)) {
            nRungs++;
        } else {
            break;
        }
    }

//     if (nRungs <= m_lowestRung) {
//         if ((m_RStart[nRungs] + m_bucketwidth[nRungs] * l_bucket) > ts_arrival) {
//             if (nRungs == 0) {
//                 throw QueueException(QueueException::RUNG_NOT_FOUND);
//             } else {
//                 nRungs--;
//             }
//         }
//     }

    // found
    if (nRungs <= m_lowestRung) {
        // insert into tail of rung x, bucket k
        boost::uint32_t l_bucket = bucket(ts_arrival, nRungs);

        // work with inaccuracies
        // ddahlem: could still cause problems if the inaccuracies occur on lower rungs as well
        if (gsl_fcmp(ts_arrival, m_RStart[nRungs] + m_bucketwidth[nRungs] * l_bucket, 1e-9) == -1) {
            throw QueueException(QueueException::RUNG_NOT_FOUND);
        }

#ifndef NDEBUG_QUEUE
        printState(nRungs);
        std::cout << std::setprecision(14) << "Ladder -- Enter event: " << ts_arrival
                  << " between " << (m_RStart[nRungs] + m_bucketwidth[nRungs] * (l_bucket))
                  << " and " << (m_RStart[nRungs] + m_bucketwidth[nRungs] * (l_bucket + 1)) << std::endl;
#endif /* NDEBUG_QUEUE */

#ifndef NDEBUG
//         assert(m_RStart[nRungs] <= ts_arrival);
//         assert(m_RStart[nRungs] + m_bucketwidth[nRungs] * l_bucket <= ts_arrival);
//         assert(ts_arrival <= (m_RStart[nRungs] + m_bucketwidth[nRungs] * (l_bucket + 1)));
#endif /* NDEBUG */

        m_rungs[nRungs][l_bucket].push_back(*p_entry);

#ifndef NDEBUG_QUEUE
        std::cout << ", size: " << m_rungs[nRungs][l_bucket].size() << std::endl;
#endif /* NDEBUG_QUEUE */

        updateNEvents(nRungs, 1);
    } else {
        throw QueueException(QueueException::RUNG_NOT_FOUND);
    }

    return true;
}

Entry* Ladder::front() throw (QueueException)
{
    return NULL;
}

void Ladder::pop_front() throw (QueueException)
{
}


void Ladder::push(boost::uint32_t p_rung, EntryList *p_list)
{
    Entry *entry = NULL;

    // transfer one event at a time
    while (!p_list->empty()) {
        entry = reinterpret_cast<Entry*>(&p_list->front());
        boost::uint32_t l_bucket = bucket(entry->getArrival(), p_rung);
        p_list->pop_front();

#ifndef NDEBUG
//         assert(l_bucket < m_buckets[p_rung]);
//         assert(l_bucket >= m_currentBucket[p_rung]);
#endif /* NDEBUG */

#ifndef NDEBUG_QUEUE
        std::cout << std::setprecision(25) << "Ladder -- Enter event: " << entry->getArrival()
                  << " between " << (m_RStart[p_rung] + m_bucketwidth[p_rung] * (l_bucket))
                  << " and " << (m_RStart[p_rung] + m_bucketwidth[p_rung] * (l_bucket + 1))
                  << ", current bucket: " << l_bucket << std::endl;

        if (p_rung > 0) {
            printState(p_rung);
            printState(p_rung - 1);
        }
#endif /* NDEBUG_QUEUE */

        m_rungs[p_rung][l_bucket].push_back(*entry);

        updateNEvents(p_rung, 1);
    }
}


void Ladder::pushBack(EntryList *p_list)
    throw (QueueException)
{
    bool spawned = false;

    spawned = spawn(false);
    if (!spawned) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }

    push(m_lowestRung, p_list);
}


void Ladder::push(EntryList *p_list, double p_maxTS, double p_minTS)
    throw (QueueException)
{
    if (getNBC() != 0) {
        throw QueueException(QueueException::EPOCH_EXISTS);
    }

    // nothing to do
    if (p_list->size() == 0) {
        return;
    }

    // init variables
    m_lowestRung = 0;
    m_bucketwidth[0] = bucketwidth(p_maxTS, p_minTS, p_list->size());
    m_RStart[0] = p_minTS;
    m_RCur[0] = p_minTS;

    // reset the current dequeue buckets
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        m_currentBucket[i] = 0;
    }

    double diff = 0.0;
    diff = (gsl_fcmp(p_maxTS, p_minTS, 1e-9) <= 0) ? (0.0) : (p_maxTS - p_minTS);

    boost::uint32_t buckets = static_cast<boost::uint32_t>(diff / m_bucketwidth[0]);

    if (buckets >= m_BucketsFirstRung) {
        resizeFirstRung(buckets);
    }

    // transfer events
    push(m_lowestRung, p_list);
}

EntryList* Ladder::delist() throw (QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- Delist" << std::endl;
#endif /* NDEBUG_QUEUE */

    // advance the dequeue bucket
    advanceDequeueBucket(true);

    // get the nodes of the list
    EntryList *temp =
        reinterpret_cast<EntryList*>(
            &(m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]));
    boost::uint32_t size = temp->size();

    updateNEvents(m_lowestRung, -size);

#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- Delist: " << m_lowestRung << ", bucket: "
              << m_currentBucket[m_lowestRung] << ", size: " << temp->size()
              << std::endl;
#endif /* NDEBUG_QUEUE */

    // once delisted we increment rcur to invalidate the previous bucket
    if ((m_lowestRung > 0) && (m_events[m_lowestRung] == 0)) {
        m_RCur[m_lowestRung] = std::numeric_limits<double>::max();
        m_RStart[m_lowestRung] = std::numeric_limits<double>::max();
        m_currentBucket[m_lowestRung] = 0;
        m_lowestRung--;
    } else {
#ifndef NDEBUG
        assert(m_RCur[m_lowestRung] < m_RCur[m_lowestRung] + m_bucketwidth[m_lowestRung]);
#endif /* NDEBUG */

        m_RCur[m_lowestRung] += m_bucketwidth[m_lowestRung];
        m_currentBucket[m_lowestRung]++;
    }

    return temp;
}

void Ladder::resizeFirstRung(boost::uint32_t p_base)
{
    // resize the rung size
    m_BucketsFirstRung = 2 * p_base;
    m_buckets[0] = m_BucketsFirstRung;

    m_rungs[0] = EntryListSA(new EntryList[m_BucketsFirstRung]);
}

void Ladder::advanceDequeueBucket(bool p_spawn) throw (QueueException)
{
#ifndef NDEBUG_QUEUE
    std::cout << "Ladder -- advance dequeue bucket" << std::endl;
#endif /* NDEBUG_QUEUE */

    boost::uint32_t elements = 0;

    // skip empty tail buckets of the current and lower rungs
    while (true) {
        if ((m_lowestRung > 0) && (m_events[m_lowestRung] == 0)) {

#ifndef NDEBUG
            assert(m_events[m_lowestRung] == 0);
#endif /* NDEBUG */

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
#endif /* NDEBUG_QUEUE */

#ifndef NDEBUG
        if (m_lowestRung == 0) {
            if ((m_currentBucket[m_lowestRung] >= m_BucketsFirstRung)
                && (m_NBC > 0)) {
                std::cout << "current bucket: " << m_currentBucket[m_lowestRung] << std::endl;

                for (boost::uint16_t i = 0; i < m_NRung; ++i) {
                    boost::uint16_t buckets = m_buckets[i];

                    if (m_events[i] > 0) {
                        std::cout << i << ", elements " << m_events[i] << std::endl;
                    }
                    for (boost::uint16_t j = 0; j < buckets; ++j) {
                        if (m_rungs[i][j].size() > 0) {
                            std::cout << i << ", " << j << std::endl;
                        }
                    }
                }
            }
        }
        assert(m_currentBucket[m_lowestRung] <= m_buckets[m_lowestRung]);

#endif /* NDEBUG */

        // how many elements are in this bucket?
        elements = m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]].size();

#ifndef NDEBUG_QUEUE
        std::cout << "Ladder -- rung " << m_lowestRung
                  << ", num events " << elements << " in bucket "
                  << m_currentBucket[m_lowestRung] << std::endl;
#endif /* NDEBUG_QUEUE */

        // advance to next bucket
        if (elements == 0) {
#ifndef NDEBUG
            assert(m_RCur[m_lowestRung] < m_RCur[m_lowestRung] + m_bucketwidth[m_lowestRung]);
#endif /* NDEBUG */

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
#endif /* NDEBUG_QUEUE */
}

bool Ladder::canAdvance()
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

bool Ladder::spawn(bool p_doEnlist)
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

        // journal on the Ladder Queue by Tang, et. al.
        m_bucketwidth[m_lowestRung] =
            m_bucketwidth[m_lowestRung - 1] / (m_Thres  - 1);
        m_RStart[m_lowestRung] =
            m_RCur[m_lowestRung - 1];
        m_RCur[m_lowestRung] =
            m_RStart[m_lowestRung];

        // we need to set the current dequeue bucket of the spawned rung to zero
        m_currentBucket[m_lowestRung] = 0;


        if (p_doEnlist) {
#ifndef NDEBUG
            if ((m_lowestRung - 1) == 0) {
                assert(m_currentBucket[m_lowestRung - 1] < m_BucketsFirstRung);
            } else {
                assert(m_currentBucket[m_lowestRung - 1] < m_Thres);
            }
#endif /* NDEBUG */

            // copy the elements from the previous bucket to the next rung
            EntryList *list =
                reinterpret_cast<EntryList*>(
                    &(m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]));

            updateNEvents(m_lowestRung - 1, -list->size());
            push(m_lowestRung, list);
        }

#ifndef NDEBUG
        assert(m_RCur[m_lowestRung - 1] < m_RCur[m_lowestRung - 1] + m_bucketwidth[m_lowestRung - 1]);
#endif /* NDEBUG */

        m_currentBucket[m_lowestRung - 1]++;
        m_RCur[m_lowestRung - 1] += m_bucketwidth[m_lowestRung - 1];

        result = true;
    }

    return result;
}

void Ladder::createRung()
{
    m_NRung++;

    // create a new rung
    EntryListSM rungs = EntryListSM(new EntryListSA[m_NRung]);

    // copy the old rungs over
    for (boost::uint32_t i = 0; i < (m_NRung - 1); ++i) {
        rungs[i] = m_rungs[i];
    }
    rungs[m_NRung - 1] = EntryListSA(new EntryList[m_Thres]);

    m_rungs = rungs;

    // resize the events data structure
    boost::uint32_t *events = new boost::uint32_t[m_NRung];
    memset(events, 0, sizeof(boost::uint32_t) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        events[i] = m_events[i];
    }

    m_events = tIntSA(events);

    // resize the currentBucket data structure
    boost::uint32_t *currentBucket = new boost::uint32_t[m_NRung];
    memset(currentBucket, 0, sizeof(boost::uint32_t) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        currentBucket[i] = m_currentBucket[i];
    }

    m_currentBucket = tIntSA(currentBucket);

    // resize the buckets data structure
    boost::uint32_t *buckets = new boost::uint32_t[m_NRung];
    memset(buckets, 0, sizeof(boost::uint32_t) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        buckets[i] = m_buckets[i];
    }

    m_buckets = tIntSA(buckets);

    // resize the bucketwidth data structure
    double *bucketwidth = new double[m_NRung];
    memset(bucketwidth, 0, sizeof(double) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        bucketwidth[i] = m_bucketwidth[i];
    }

    m_bucketwidth = tDoubleSA(bucketwidth);

    // resize the rcur data structure
    double *rcur = new double[m_NRung];
    memset(rcur, 0, sizeof(double) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        rcur[i] = m_RCur[i];
    }

    m_RCur = tDoubleSA(rcur);

    // resize the rstart data structure
    double *rstart = new double[m_NRung];
    memset(rstart, 0, sizeof(double) * m_NRung);
    for (boost::uint32_t i = 0; i < m_NRung; ++i) {
        rstart[i] = m_RStart[i];
    }

    m_RStart = tDoubleSA(rstart);
}

#if !defined(NDEBUG) || !defined(NDEBUG_QUEUE)
void Ladder::printState(boost::uint32_t p_rung)
{
    std::cout << std::setprecision(21) << "Ladder State -- Rung: " << p_rung
              << ", current bucket: " << m_currentBucket[p_rung]
              << ", start: " << m_RStart[p_rung]
              << ", rcur: " << m_RCur[p_rung]
              << ", bucketwidth: " << m_bucketwidth[p_rung]
              << std::endl;
}
#endif /* NDEBUG || NDEBUG_QUEUE */

}
}
