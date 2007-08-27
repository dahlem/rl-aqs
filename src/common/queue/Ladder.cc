#include <cmath>
#include <cstddef>

#include "Ladder.hh"



Ladder::Ladder() 
{
    m_Thres = 50;
    m_NRung = 5;
    m_NBC = 0;

    *m_rungs = new Fifo[m_NRung];
    m_bucketwidth = new double[m_NRung];
    m_RCur = new double[m_NRung];
    m_RStart = new double[m_NRung];
    
    // initialise the allocated memory
    memset(m_rungs, 0, sizeof(Fifo) * m_NRung);
    memset(m_bucketwidth, 0, sizeof(double) * m_NRung);
    memset(m_RCur, 0, sizeof(double) * m_NRung);
    memset(m_RStart, 0, sizeof(double) * m_NRung);

    for (int i = 1; i < m_NRung; ++i) {
        m_rungs[i] = new Fifo[m_Thres];
        memset(m_rungs[i], 0, sizeof(Fifo) * m_Thres);
    }
}

Ladder::~Ladder() 
{
    for (int i = 0; i < m_NRung; ++i) {
        delete[] m_rungs[i];
    }
    
    delete[] m_rungs;
    delete[] m_bucketwidth;
    delete[] m_RCur;
    delete[] m_RStart;
    
}

void Ladder::enqueue(entry_t *const p_entry) throw (QueueException)
{
    int nRungs = 0;

    // find the rung
    while ((p_entry->arrival < getRCur(nRungs)) && (nRungs <= getNRung())) {
        nRungs++;
    }

    // found
    if (nRungs <= getNRung()) {
        int bucket_k = (int) floor((p_entry->arrival - getRStart(nRungs))
                                   / getBucketwidth(nRungs));

        // insert into tail of rung x, bucket k
    } else {
        throw QueueException(QueueException::RUNG_NOT_FOUND);
    }
}

entry_t *const Ladder::dequeue() 
{
    return NULL;
}

void Ladder::enlist(node_double_t *p_list, long p_size) 
{}

node_double_t *Ladder::delist() 
{
    return NULL;
}

double Ladder::getBucketwidth(int p_rung) 
{
    return NULL;
}

int Ladder::getNBC() 
{
    return NULL;
}

int Ladder::getNBucket(int p_bucket, int p_rung) 
{
    return NULL;
}

int Ladder::getNRung() 
{
    return NULL;
}

int Ladder::getThres() 
{
    return NULL;
}

double Ladder::getRCur(int p_rung) 
{
    return NULL;
}

double Ladder::getRStart(int p_rung) 
{
    return NULL;
}

