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

/** @file Ladder.cc
 * Implementation of the ladder structure @ref{Ladder.hh} of the Ladder Queue.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

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

#include "Ladder.hh"
namespace dcommon = des::common;



dcommon::Ladder::Ladder(boost::int32_t p_thres)
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

    m_events = dcommon::tIntSA(new boost::int32_t[m_NRung]);
    m_currentBucket = dcommon::tIntSA(new boost::int32_t[m_NRung]);
    m_bucketwidth = dcommon::tDoubleSA(new double[m_NRung]);
    m_RCur = dcommon::tDoubleSA(new double[m_NRung]);
    m_RStart = dcommon::tDoubleSA(new double[m_NRung]);

    // initialise the allocated memory
    memset(m_events.get(), 0, sizeof(boost::int32_t) * m_NRung);
    memset(m_currentBucket.get(), 0, sizeof(boost::int32_t) * m_NRung);
    memset(m_bucketwidth.get(), 0, sizeof(double) * m_NRung);
    memset(m_RCur.get(), 0, sizeof(double) * m_NRung);
    memset(m_RStart.get(), 0, sizeof(double) * m_NRung);

    m_rungs = dcommon::EntryListSM(new dcommon::EntryListSA[m_NRung]);

    for (boost::int32_t i = 0; i < m_NRung; ++i) {
        m_rungs[i] = dcommon::EntryListSA(new dcommon::EntryList[m_Thres]);
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
    for (boost::int32_t i = 0; i < m_BucketsFirstRung; ++i) {
        dcommon::EntryList *list =
            reinterpret_cast<dcommon::EntryList*>(
                &(m_rungs[0][i]));
        list->erase_and_dispose(list->begin(), list->end(),
                               dcommon::delete_disposer());
    }

    for (boost::int32_t i = 1; i < m_NRung; ++i) {
        for (boost::int32_t j = 0; j < m_Thres; ++j) {
            dcommon::EntryList *list =
                reinterpret_cast<dcommon::EntryList*>(
                    &(m_rungs[i][j]));
            list->erase_and_dispose(list->begin(), list->end(),
                                    dcommon::delete_disposer());
        }
    }
}

#ifdef HAVE_LADDERSTATS
void dcommon::Ladder::record()
{
    (*os.get()) << events_in << "," << events_out << "," << getNBC() << getNRung() << std::endl;

    // reset the stat fields
    events_in = 0;
    events_out = 0;
}
#endif /* HAVE_LADDERSTATS */


double dcommon::Ladder::getBucketwidth(boost::int32_t p_rung) throw (dcommon::QueueException)
{
    if (p_rung >= m_NRung) {
        throw dcommon::QueueException(dcommon::QueueException::RUNG_OUT_OF_BOUNDS);
    }

    return m_bucketwidth[p_rung];
}

double dcommon::Ladder::getBucketwidth()
{
    return m_bucketwidth[m_lowestRung];
}

boost::int32_t dcommon::Ladder::getNBC()
{
    return m_NBC;
}

boost::int32_t dcommon::Ladder::getNBucket(boost::int32_t p_rung, boost::int32_t p_bucket) throw (dcommon::QueueException)
{
    if (p_rung >= m_NRung) {
        throw dcommon::QueueException(dcommon::QueueException::RUNG_OUT_OF_BOUNDS);
    }
    if (p_rung == 0) {
        if (p_bucket >= m_BucketsFirstRung) {
            throw dcommon::QueueException(dcommon::QueueException::BUCKET_OUT_OF_BOUNDS);
        }
    } else {
        if (p_bucket >= m_Thres) {
            throw dcommon::QueueException(dcommon::QueueException::BUCKET_OUT_OF_BOUNDS);
        }
    }

    return m_rungs[p_rung][p_bucket].size();
}

boost::int32_t dcommon::Ladder::getNBucket()
{
    return m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]].size();
}

boost::int32_t dcommon::Ladder::getNRung()
{
    return m_NRung;
}

boost::int32_t dcommon::Ladder::getThres()
{
    return m_Thres;
}

double dcommon::Ladder::getRCur(boost::int32_t p_rung)
{
    if (p_rung >= m_NRung) {
        throw dcommon::QueueException(dcommon::QueueException::RUNG_OUT_OF_BOUNDS);
    }
    return m_RCur[p_rung];
}

double dcommon::Ladder::getRCur()
{
    return m_RCur[m_lowestRung];
}

double dcommon::Ladder::getRStart(boost::int32_t p_rung)
{
    if (p_rung >= m_NRung) {
        throw dcommon::QueueException(dcommon::QueueException::RUNG_OUT_OF_BOUNDS);
    }
    return m_RStart[p_rung];
}

double dcommon::Ladder::bucketwidth(double p_max, double p_min, boost::int32_t p_n)
{
    if (p_max == p_min) {
        return 1.0;
    } else {
        return (p_max - p_min) / p_n;
    }
}

boost::int32_t dcommon::Ladder::bucket(double p_TS, boost::int32_t p_rung)
{
    double diff = (p_TS - getRStart(p_rung));
    double result = diff / getBucketwidth(p_rung);
    boost::int32_t retVal = (boost::int32_t) floor(result);

    return retVal;
}

bool dcommon::Ladder::push(dcommon::Entry *p_entry) throw (dcommon::QueueException)
{
    // cannot enqueue, if the internal structure has not been initialised
    // by an epoch
    if (getNBucket() == 0) {
        throw dcommon::QueueException(dcommon::QueueException::NO_EPOCH_INIT);
    }

    boost::int32_t nRungs = 0;

    // find the rung
    while ((p_entry->arrival < getRCur(nRungs)) && (nRungs <= m_lowestRung)) {
        nRungs++;
    }

    // found
    if (nRungs <= m_lowestRung) {
        // insert into tail of rung x, bucket k
        m_rungs[nRungs][bucket(p_entry->arrival, nRungs)].push_back(*p_entry);
        m_events[nRungs]++;

#ifdef HAVE_LADDERSTATS
        events_in++;
#endif /* HAVE_LADDERSTATS */
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


void dcommon::Ladder::push(boost::int32_t p_rung, dcommon::EntryList *p_list)
{
    dcommon::Entry *entry = NULL;

    // transfer one event at a time
    while (!p_list->empty()) {
        entry = reinterpret_cast<dcommon::Entry*>(&p_list->front());
        p_list->pop_front();
        m_rungs[p_rung][bucket(entry->arrival, p_rung)].push_back(*entry);
        m_events[p_rung]++;

#ifdef HAVE_LADDERSTATS
        events_in++;
#endif /* HAVE_LADDERSTATS */
    }
}

void dcommon::Ladder::pushBack(dcommon::EntryList *p_list)
    throw (dcommon::QueueException)
{
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
    if (getNBucket() != 0) {
        throw dcommon::QueueException(dcommon::QueueException::EPOCH_EXISTS);
    }

    // nothing to do
    if (p_list->size() == 0) {
        return;
    }

    // init variables
    m_bucketwidth[0] = bucketwidth(p_maxTS, p_minTS, p_list->size());
    m_RStart[0] = p_minTS;
    m_RCur[0] = p_minTS + getBucketwidth(0); // different in paper

    boost::int32_t buckets = static_cast<boost::int32_t>(
        (p_maxTS - p_minTS) / m_bucketwidth[0]);
    if (buckets >= m_BucketsFirstRung) {
        resizeFirstRung(buckets);
    }

    // transfer events
    push(m_lowestRung, p_list);
}

dcommon::EntryList* const dcommon::Ladder::delist()
{
    // advance the dequeue bucket
    advanceDequeueBucket(1);

    // get the nodes of the list
    dcommon::EntryList *temp =
        reinterpret_cast<dcommon::EntryList*>(
            &(m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]));
    boost::int32_t size = temp->size();

#ifdef HAVE_LADDERSTATS
    events_out += size;
#endif /* HAVE_LADDERSTATS */

    // keep track of the events in each rung
    m_events[m_lowestRung] -= size;

    return temp;
}

void dcommon::Ladder::resizeFirstRung(boost::int32_t p_base)
{
    // resize the rung size
    m_BucketsFirstRung = 2 * p_base;
    m_rungs[0].reset(new dcommon::EntryList[m_BucketsFirstRung]);
}

void dcommon::Ladder::advanceDequeueBucket(bool p_spawn)
{
    boost::int32_t elements = 0;

    // skip empty tail buckets of the current and lower rungs
    while ((m_lowestRung > 0) && (m_events[m_lowestRung] == 0)) {
        m_lowestRung--;
    }

    // find next non-empty bucket from lowest rung
    while (true) {
        if (!canAdvance()) {
            break;
        }

        // how many elements are in this bucket?
        elements = m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]].size();

        // advance to next bucket
        if (elements == 0) {
            m_currentBucket[m_lowestRung]++;
            m_RCur[m_lowestRung] += getBucketwidth(m_lowestRung);
        } else {
            break;
        }
    }

    if (p_spawn && (elements > m_Thres)) {
        advanceDequeueBucket(spawn(true));
    }
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
        if (m_currentBucket[m_lowestRung] == m_Thres) {
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

        // these formulas are different from the ones presented in the TOMACS
        // journal on the Ladder Queue by Tang, et. al.
        m_bucketwidth[m_lowestRung] =
            getBucketwidth(m_lowestRung - 1) / (m_Thres  - 1);
        m_RStart[m_lowestRung] =
            getRCur(m_lowestRung - 1)
            - getBucketwidth(m_lowestRung - 1);
        m_RCur[m_lowestRung] =
            getRCur(m_lowestRung - 1)
            - getBucketwidth(m_lowestRung - 1)
            + getBucketwidth(m_lowestRung);

        if (p_doEnlist) {
            // copy the elements from the previous bucket to the next rung
            dcommon::EntryList *list =
                reinterpret_cast<dcommon::EntryList*>(
                    &(m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]));
            m_events[m_lowestRung - 1] -= list->size();

            push(m_lowestRung, list);
        }

        result = true;
    }

    return result;
}

void dcommon::Ladder::createRung()
{
    m_NRung++;

    // create a new rung
    dcommon::EntryListSM rungs = dcommon::EntryListSM(new dcommon::EntryListSA[m_NRung]);

    for (boost::int32_t i = 0; i < m_NRung; ++i) {
        rungs[i] = dcommon::EntryListSA(new dcommon::EntryList[m_Thres]);
    }

    // copy the old rungs over
    for (boost::int32_t i = 0; i < m_lowestRung; ++i) {
        rungs[i].swap(m_rungs[i]);
    }

    m_rungs = rungs;

    // resize the events data structure
    boost::int32_t *events = new boost::int32_t[m_NRung];
    memset(events, 0, sizeof(boost::int32_t) * m_NRung);
    for (boost::int32_t i = 0; i < m_lowestRung; ++i) {
        events[i] = m_events[i];
    }

    m_events.reset(events);

    // resize the currentBucket data structure
    boost::int32_t *currentBucket = new boost::int32_t[m_NRung];
    memset(currentBucket, 0, sizeof(boost::int32_t) * m_NRung);
    for (boost::int32_t i = 0; i < m_lowestRung; ++i) {
        currentBucket[i] = m_currentBucket[i];
    }

    m_currentBucket.reset(currentBucket);

    // resize the bucketwidth data structure
    double *bucketwidth = new double[m_NRung];
    memset(bucketwidth, 0, sizeof(double) * m_NRung);
    for (boost::int32_t i = 0; i < m_lowestRung; ++i) {
        bucketwidth[i] = m_bucketwidth[i];
    }

    m_bucketwidth.reset(bucketwidth);

    // resize the rcur data structure
    double *rcur = new double[m_NRung];
    memset(rcur, 0, sizeof(double) * m_NRung);
    for (boost::int32_t i = 0; i < m_lowestRung; ++i) {
        rcur[i] = m_RCur[i];
    }

    m_RCur.reset(rcur);

    // resize the rstart data structure
    double *rstart = new double[m_NRung];
    memset(rstart, 0, sizeof(double) * m_NRung);
    for (boost::int32_t i = 0; i < m_lowestRung; ++i) {
        rstart[i] = m_RStart[i];
    }

    m_RStart.reset(rstart);
}
