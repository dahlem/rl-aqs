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
#include <cstring>

#include "Ladder.hh"
using des::common::Ladder;



Ladder::Ladder(int p_thres)
{
    m_Thres = p_thres;
    init();
}

Ladder::Ladder()
{
    m_Thres = DEFAULT_THRES;
    init();
}

void Ladder::init()
{
    m_NRung = 5;
    m_lowestRung = 0;
    m_NBC = 0;
    m_BucketsFirstRung = m_Thres;

    m_events = new int[m_NRung];
    m_currentBucket = new int[m_NRung];
    m_bucketwidth = new double[m_NRung];
    m_RCur = new double[m_NRung];
    m_RStart = new double[m_NRung];

    // initialise the allocated memory
    memset(m_events, 0, sizeof(int) * m_NRung);
    memset(m_currentBucket, 0, sizeof(int) * m_NRung);
    memset(m_bucketwidth, 0, sizeof(double) * m_NRung);
    memset(m_RCur, 0, sizeof(double) * m_NRung);
    memset(m_RStart, 0, sizeof(double) * m_NRung);

    m_rungs = new Fifo**[m_NRung];

    for (int i = 0; i < m_NRung; ++i) {
        m_rungs[i] = new Fifo*[m_Thres];
        for (int j = 0; j < m_Thres; ++j) {
            m_rungs[i][j] = new Fifo();
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
    for (int j = 0; j < m_BucketsFirstRung; ++j) {
        delete m_rungs[0][j];
    }
    delete[] m_rungs[0];
    m_rungs[0] = NULL;

    for (int i = 1; i < m_NRung; ++i) {
        for (int j = 0; j < m_Thres; ++j) {
            delete m_rungs[i][j];
        }
        delete[] m_rungs[i];
        m_rungs[i] = NULL;
    }

    delete[] m_rungs;
    delete[] m_bucketwidth;
    delete[] m_RCur;
    delete[] m_RStart;
    delete[] m_events;
    delete[] m_currentBucket;
}

#ifdef HAVE_LADDERSTATS
void Ladder::record()
{
    (*os.get()) << events_in << "," << events_out << "," << getNBC() << getNRung() << std::endl;

    // reset the stat fields
    events_in = 0;
    events_out = 0;
}
#endif /* HAVE_LADDERSTATS */


double Ladder::getBucketwidth(int p_rung) throw (QueueException)
{
    if (p_rung >= m_NRung) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }

    return m_bucketwidth[p_rung];
}

double Ladder::getBucketwidth()
{
    return m_bucketwidth[m_lowestRung];
}

int Ladder::getNBC()
{
    return m_NBC;
}

long Ladder::getNBucket(int p_rung, int p_bucket) throw (QueueException)
{
    if (p_rung >= m_NRung) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }
    if (p_rung == 0) {
        if (p_bucket >= m_BucketsFirstRung) {
            throw QueueException(QueueException::BUCKET_OUT_OF_BOUNDS);
        }
    } else {
        if (p_bucket >= m_Thres) {
            throw QueueException(QueueException::BUCKET_OUT_OF_BOUNDS);
        }
    }

    return m_rungs[p_rung][p_bucket]->size();
}

long Ladder::getNBucket()
{
    return m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]->size();
}

int Ladder::getNRung()
{
    return m_NRung;
}

int Ladder::getThres()
{
    return m_Thres;
}

double Ladder::getRCur(int p_rung)
{
    if (p_rung >= m_NRung) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }
    return m_RCur[p_rung];
}

double Ladder::getRCur()
{
    return m_RCur[m_lowestRung];
}

double Ladder::getRStart(int p_rung)
{
    if (p_rung >= m_NRung) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }
    return m_RStart[p_rung];
}

double Ladder::bucketwidth(double p_max, double p_min, long p_n)
{
    if (p_max == p_min) {
        return 1.0;
    } else {
        return (p_max - p_min) / p_n;
    }
}

int Ladder::bucket(double p_TS, int p_rung)
{
    double diff = (p_TS - getRStart(p_rung));
    double result = diff / getBucketwidth(p_rung);
    int retVal = (int) floor(result);

    return retVal;
}

void Ladder::pushBack(node_double_t *p_list, long p_size)
{
    enlist(m_lowestRung, p_list, p_size);

    // advance dequeue bucket
    advanceDequeueBucket(1);
}

void Ladder::enqueue(entry_t *const p_entry) throw (QueueException)
{
    // cannot enqueue, if the internal structure has not been initialised
    // by an epoch
    if (getNBucket() == 0) {
        throw QueueException(QueueException::NO_EPOCH_INIT);
    }

    int nRungs = 0;

    // find the rung
    while ((p_entry->arrival < getRCur(nRungs)) && (nRungs <= m_lowestRung)) {
        nRungs++;
    }

    // found
    if (nRungs <= m_lowestRung) {
        // insert into tail of rung x, bucket k
        node_double_t *newNode = new node_double_t(p_entry, NULL, NULL);
        enlist(nRungs, newNode, 1);
    } else {
        throw QueueException(QueueException::RUNG_NOT_FOUND);
    }
}

entry_t *const Ladder::dequeue()
{
    return NULL;
}

void Ladder::enlist(int p_rung, node_double_t *p_list, long p_size)
{
    // transfer one event at a time
    for (int i = 0; i < p_size; ++i) {
        m_rungs[p_rung][bucket(p_list->data->arrival, p_rung)]->enlist(p_list, 1);
        m_events[p_rung]++;
        p_list = p_list->next;

#ifdef HAVE_LADDERSTATS
        events_in++;
#endif /* HAVE_LADDERSTATS */
    }
}

void Ladder::enlist(node_double_t *p_list, long p_size)
{
    // resize if necessary
    if (p_size >= m_BucketsFirstRung) {
        resizeFirstRung(p_size);
    }

    enlist(0, p_list, p_size);
}

void Ladder::enlist(node_double_t *p_list, long p_size,
                    double p_maxTS, double p_minTS)
    throw (QueueException)
{
    if (getNBucket() != 0) {
        throw QueueException(QueueException::EPOCH_EXISTS);
    }

    // nothing to do
    if (p_size == 0) {
        return;
    }

    // init variables
    m_bucketwidth[0] = bucketwidth(p_maxTS, p_minTS, p_size);
    m_RStart[0] = p_minTS;
    m_RCur[0] = p_minTS + getBucketwidth(0);

    // transfer events
    enlist(p_list, p_size);

    // advance dequeue bucket
    advanceDequeueBucket(1);
}

node_double_t *Ladder::delist()
{
#ifdef HAVE_LADDERSTATS
    events_out += m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]->size();
#endif /* HAVE_LADDERSTATS */

    // keep track of the events in each rung
    m_events[m_lowestRung] -= m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]->size();

    // get the nodes of the list
    node_double_t *temp = m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]->delist();

    // advance the current dequeue bucket
    advanceDequeueBucket(1);

    return temp;
}

void Ladder::resizeFirstRung(int p_base)
{
    // delete the individual elements
    for (int i = 0; i < m_BucketsFirstRung; ++i) {
        delete m_rungs[0][i];
    }

    // delete the first rung
    delete[] m_rungs[0];

    // resize the rung size
    m_BucketsFirstRung = 2 * p_base;
    m_rungs[0] = new Fifo*[m_BucketsFirstRung];

    // create new fifos
    for (int i = 0; i < m_BucketsFirstRung; ++i) {
        m_rungs[0][i] = new Fifo();
    }
}

void Ladder::advanceDequeueBucket(bool p_spawn)
{
    int elements = 0;

    // skip empty tail buckets of the current and lower rungs
    while ((m_lowestRung > 0) && (m_events[m_lowestRung] == 0)) {
        m_lowestRung--;
    }

    // find next non-empty bucket from lowest rung
    while (true) {
        // leave, if the current bucket cannot be advanced further
        // two cases: (1) we are on the first rung; (2) we are between 0-m_NRung
        if (m_lowestRung == 0) {
            if (m_currentBucket[m_lowestRung] == (m_BucketsFirstRung - 1)) {
                break;
            }
        } else if (m_lowestRung < m_NRung) {
            if (m_currentBucket[m_lowestRung] == m_Thres) {
                break;
            }
        }

        // how many elements are in this bucket?
        elements = m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]->size();

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
            int size = m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]->size();
            m_events[m_lowestRung - 1] -= size;

            enlist(
                m_lowestRung,
                m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]->delist()->next,
                size);
        }

        result = true;
    }

    return result;
}

void Ladder::createRung()
{
    m_NRung++;

    // create a new rung
    Fifo*** rungs = new Fifo**[m_NRung];

    rungs[m_lowestRung] = new Fifo*[m_Thres];
    for (int j = 0; j < m_Thres; ++j) {
        rungs[m_lowestRung][j] = new Fifo();
    }

    // copy the old rungs over
    for (int i = 0; i < m_lowestRung; ++i) {
        rungs[i] = m_rungs[i];
    }

    delete[] m_rungs;
    m_rungs = rungs;

    // resize the events data structure
    int *events = new int[m_NRung];
    memset(events, 0, sizeof(int) * m_NRung);
    for (int i = 0; i < m_lowestRung; ++i) {
        events[i] = m_events[i];
    }

    delete[] m_events;
    m_events = events;

    // resize the currentBucket data structure
    int *currentBucket = new int[m_NRung];
    memset(currentBucket, 0, sizeof(int) * m_NRung);
    for (int i = 0; i < m_lowestRung; ++i) {
        currentBucket[i] = m_currentBucket[i];
    }

    delete[] m_currentBucket;
    m_currentBucket = currentBucket;

    // resize the bucketwidth data structure
    double *bucketwidth = new double[m_NRung];
    memset(bucketwidth, 0, sizeof(double) * m_NRung);
    for (int i = 0; i < m_lowestRung; ++i) {
        bucketwidth[i] = m_bucketwidth[i];
    }

    delete[] m_bucketwidth;
    m_bucketwidth = bucketwidth;

    // resize the rcur data structure
    double *rcur = new double[m_NRung];
    memset(rcur, 0, sizeof(double) * m_NRung);
    for (int i = 0; i < m_lowestRung; ++i) {
        rcur[i] = m_RCur[i];
    }

    delete[] m_RCur;
    m_RCur = rcur;

    // resize the rstart data structure
    double *rstart = new double[m_NRung];
    memset(rstart, 0, sizeof(double) * m_NRung);
    for (int i = 0; i < m_lowestRung; ++i) {
        rstart[i] = m_RStart[i];
    }

    delete[] m_RStart;
    m_RStart = rstart;
}
