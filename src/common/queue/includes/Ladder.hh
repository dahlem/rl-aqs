// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LADDER_HH
#define LADDER_HH


#include "Fifo.hh"
#include "List.hh"
#include "Queue.hh"
#include "QueueException.hh"



/**
 * This class represents the ladder structure of the <code>LadderQueue</code>.
 * The elements int this structure are distributed uniformly in the limit
 * among the rungs of the ladder.</p>
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Ladder: public Queue, List
{
public:
    Ladder();
    Ladder(int p_thres);
    ~Ladder();

    /**
     * @see Queue#enqueue(entry_t* const) throw (QueueException)
     */
    void enqueue(entry_t *const p_entry) throw (QueueException);

    /**
     * @see Queue#dequeue()
     */
    entry_t *const dequeue();

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
    void enlist(node_double_t *p_list, long p_size,
                double p_maxTS, double p_minTS) throw (QueueException);

    /**
     * @see List#delist()
     */
    node_double_t *delist();

    /**
     * @return the number of events in the ladder structure
     */
    int getNBC();

    /**
     * The current number of instantiated rungs
     */
    int getNRung();

    /**
     * The event threshold of each bucket in the rung
     */
    int getThres();

    /**
     * @param int the rung
     * @param int the bucket
     * @return the number of events in a given bucket in a given rung
     */
    long getNBucket(int p_rung, int p_bucket) throw (QueueException);

    /**
     * @return the number of events in the current dequeue bucket
     */
    long getNBucket();

    /**
     * @param int rung
     * @return the bucketwidth of a given rung
     * @throws QueueException thrown, if the given rung has not been initialised
     */
    double getBucketwidth(int p_rung) throw (QueueException);

    /**
     * @return the bucketwidth of the currently active dequeue rung
     */
    double getBucketwidth();

    /**
     * @param int the rung
     * @return the minimum arrival timestamp of a given rung for enqueuing events
     */
    double getRCur(int p_rung);
    
    /**
     * @return the minimum arrival timestamp of a given rung for enqueuing events
     *         on the currently active rung
     */
    double getRCur();

    /**
     * @param int the rung
     * @return the starting timestamp of a given rung
     */
    double getRStart(int p_rung);

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
     */
    void pushBack(node_double_t *p_list, long p_size);


private:
    /**
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(node_double_t *p_list, long p_size);

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
    double bucketwidth(double p_max, double p_min, long p_n);

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
    int bucket(double p_TS, int p_rung);

    /**
     * This method resizes the first rung given the number of events to be
     * inserted in it. the <code>resizeFirstRung</code> operation is can only
     * be called at the start of an epoch.
     *
     * @param int the number of the events to be enqueued into the first rung
     */
    void resizeFirstRung(int p_size);

    /**
     * This method advances the current dequeue bucket to the next non-empty
     * bucket. At the same time the <code>m_RCur</code> value is adjusted.
     *
     * @param bool true, if a spawn operation can/should be initiated to
     *             re-distribute the events to a lower rung.
     */
    void advanceDequeueBucket(bool p_spawn);

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
     * Insert a list into a specified rung.
     * 
     * @param the rung
     * @param the list
     * @param the size of the list
     * @see List#enlist(node_double_t*, long)
     */
    void enlist(int p_rung, node_double_t *p_list, long p_size);


    /**
     * Default constant of the event threshold per bucket.
     */
    static const int DEFAULT_THRES = 50;

    /**
     * Constang specifying the maximum number of rungs.
     */
    static const int MAX_RUNGS = 8;

    Fifo ***m_rungs;
    int m_BucketsFirstRung;

    int m_NRung;
    int m_NBC;
    int m_lowestRung;
    int m_Thres;

    int *m_events;
    int *m_currentBucket;

    double *m_bucketwidth;
    double *m_RCur;
    double *m_RStart;
};


#endif
