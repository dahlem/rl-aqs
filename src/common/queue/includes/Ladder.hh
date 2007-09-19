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



class Ladder: public Queue, List
{
public:
    Ladder();
    Ladder(int p_thres);
    ~Ladder();

    void enqueue(entry_t *const p_entry) throw (QueueException);
    entry_t *const dequeue();

    void enlist(node_double_t *p_list, long p_size,
                double p_maxTS, double p_minTS) throw (QueueException);
    node_double_t *delist();

    int getNBC();
    int getNRung();
    int getThres();

    long getNBucket(int p_rung, int p_bucket);
    long getNBucket();

    double getBucketwidth(int p_rung) throw (QueueException);
    double getBucketwidth();

    double getRCur(int p_rung);
    double getRCur();

    double getRStart(int p_rung);

    bool spawn(bool p_doEnlist);
    void pushBack(node_double_t *p_list, long p_size);


private:
    void enlist(node_double_t *p_list, long p_size);
    double bucketwidth(double p_max, double p_min, long p_n);
    int bucket(double p_TS, int p_rung);
    void resizeFirstRung(int p_size);
    void advanceDequeueBucket(bool p_spawn);
    void createRung();
    void init();
    void enlist(int p_rung, node_double_t *p_list, long p_size);


    static const int DEFAULT_THRES = 50;
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
