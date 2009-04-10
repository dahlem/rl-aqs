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

/** @file Ladder.hh
 * Declaration of the ladder structure of the Ladder Queue.
 */
#ifndef __LADDER_HH__
#define __LADDER_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include "Entry.hh"
#include "Queue.hh"
#include "QueueException.hh"
namespace dcommon = des::common;



namespace des
{
    namespace common
    {

/** @typedef tIntSA
 * a type defintion of a shared array of ints
 */
typedef boost::shared_array <boost::uint32_t> tIntSA;

/** @typedef tDoubleSA
 * a type defintion of a shared array of doubles
 */
typedef boost::shared_array <double> tDoubleSA;



/**
 * This class represents the ladder structure of the <code>LadderQueue</code>.
 * The elements int this structure are distributed uniformly in the limit
 * among the rungs of the ladder.</p>
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Ladder: public dcommon::Queue
{
public:
    Ladder();
    Ladder(boost::uint32_t p_thres);
    ~Ladder();

#ifdef HAVE_LADDERSTATS
    void record();
#endif /* HAVE_LADDERSTATS */

    dcommon::Entry* front() throw (dcommon::QueueException);
    void pop_front() throw (dcommon::QueueException);

    const bool push(dcommon::Entry *p_entry) throw (dcommon::QueueException);
    dcommon::EntryList* const delist() throw (dcommon::QueueException);

#ifndef NDEBUG
    void printEvents();
#endif /* NDEBUG */

    /**
     * Insert a list into the ladder structure and intialise the following
     * variables:
     * \f[
     *   bucketwidth[0] = (maxTS-minTS)/n
     * \f]
     * \f[
     *   Rstart[0] = minTS
     * \f]
     * \f[
     *   Rcur[0] = minTS + bucketwidth[0]
     * \f]
     *
     * @param node_double_t* the first element of the list to be inserted
     * @param long the size of the list to be inserted
     * @double the maximum arrival timestamp of the list to be inserted
     * @double the minimum arrival timestamp of the list to be inserted
     * @see List#enlist(node_double_t*, long)
     */
    void push(dcommon::EntryList*, double p_maxTS, double p_minTS)
        throw (dcommon::QueueException);

    /**
     * @return the number of events in the ladder structure
     */
    boost::uint32_t getNBC();

    /**
     * The event threshold of each bucket in the rung
     */
    boost::uint32_t getThres();

    /**
     * The rung structure is updated according to the following equations. Those
     * do not follow the ones presented in the TOMACS journal.
     * \f$ \forall i \ge 2 \f$,
     * \f$ bucketwidth[i] = bucketwidth[i-1] / (THRES-1) \f$,
     * \f$ Rstart[i] = Rcur[i-1] - bucketwidth[i-1] \f$,
     * \f$ Rcur[i] = Rcur[i-1] - bucketwidth[i-1] + bucketwidth[i] \f$;
     *
     * @param bool specifies whether the spawn process entails copying events
     *             from a previous rung to the newly created one.
     */
    bool spawn(bool p_doEnlist);

    /**
     * This method is called in order to push back elements from the bottom
     * structure into the ladder. This happens, when the bottom structure
     * exceeds the threshold value.
     *
     * @param node_double_t* the list to be inserted
     * @param long the size of the list to be inserted
     * @throws QueueException, if the elements can not be inserted due to rungs
     *         being out of bounds.
     */
    void pushBack(dcommon::EntryList* ) throw (dcommon::QueueException);


private:
    void updateNEvents(boost::uint32_t p_rung, boost::int32_t size);

    /**
     * @see List#enlist(node_double_t*, long)
     */
    void push(dcommon::EntryList* );

    /**
     * Insert a list into a specified rung.
     *
     * @param the rung
     * @param the list
     * @param the size of the list
     * @see List#enlist(node_double_t*, long)
     */
    void push(boost::uint32_t p_rung, dcommon::EntryList* );

    /**
     * This method adapts the one presented in the TOMACS journal in order to
     * account for the rare occasion when the maximum and the minimum arrival
     * timestamps are equal.
     * \f[
     *   bucketwidth = (max-min)/n
     * \f], where
     * \f[
     *   bucketwidth = 1.0
     * \f], if \f$ max==min \f$.
     *
     * @param double the maximum arrival timestamp
     * @param double the minimum arrival timestamp
     * @param long the number of events
     * @return the bucketwidth given the max/min timestamps and the number of
     *         events.
     */
    double bucketwidth(double p_max, double p_min, boost::uint32_t p_n);

    /**
     * Determine the bucket to insert the event with a given arrival timestamp
     * into.
     * \f[
     *   bucket = \lfloor (ts - Rstart[rung]) / bucketwidth[rung] \rfloor
     * \f]
     *
     * @param double the timestamp of an event to be inserted in a rung
     * @param int the rung where the event is to be inserted to
     * @return the index of the bucket in the given rung where the event with
     *         the given arrival timestamp is to be inserted
     */
    boost::uint32_t bucket(double p_TS, boost::uint32_t p_rung);

    /**
     * This method resizes the first rung given the number of events to be
     * inserted in it. the <code>resizeFirstRung</code> operation is can only
     * be called at the start of an epoch.
     *
     * @param int the number of the events to be enqueued into the first rung
     */
    void resizeFirstRung(boost::uint32_t p_size);

    /**
     * This method advances the current dequeue bucket to the next non-empty
     * bucket. At the same time the <code>m_RCur</code> value is adjusted.
     *
     * @param bool true, if a spawn operation can/should be initiated to
     *             re-distribute the events to a lower rung.
     */
    void advanceDequeueBucket(bool p_spawn) throw (dcommon::QueueException);

    /**
     * This operation initialises a new rung, if the <code>MAX_RUNGS</code> value
     * is not exceeded and the spawn operation surpasses the currently initialised
     * rungs.
     */
    void createRung();

    /**
     * Initialise the ladder structure.
     */
    void init();

    /**
     * Check whether the dequeue bucket can be advanced further on the current rung.
     * @return true, if it can be advanced. Otherwise false.
     */
    bool canAdvance();

#if !defined(NDEBUG) || !defined(NDEBUG_QUEUE)
    /** @fn void printState(boost::uint32_t)
     * Print the current state of the given rung.
     *
     * @param boost::uint32_t the rung to be printed
     */
    void printState(boost::uint32_t p_rung);
#endif /* NDEBUG || NDEBUG_QUEUE */

    /**
     * Default constant of the event threshold per bucket.
     */
    static const boost::uint32_t DEFAULT_THRES = 50;

    /**
     * Constang specifying the maximum number of rungs.
     */
    static const boost::uint32_t MAX_RUNGS = 8;

    dcommon::EntryListSM m_rungs;

    boost::uint32_t m_BucketsFirstRung;
    boost::uint32_t m_NRung;
    boost::uint32_t m_NBC;
    boost::uint32_t m_lowestRung;
    boost::uint32_t m_Thres;

    tIntSA m_events;
    tIntSA m_currentBucket;
    tIntSA m_buckets;

    tDoubleSA m_bucketwidth;
    tDoubleSA m_RCur;
    tDoubleSA m_RStart;
};


/** @typedef tLadderSP
 * a type defintion of a shared pointer of a ladder structure
 */
typedef boost::shared_ptr <Ladder> tLadderSP;

    }
}

#endif
