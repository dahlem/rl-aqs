#include <cmath>
#include <cstddef>

#include "Ladder.hh"



Ladder::Ladder() 
{
    m_NRung = 5;
    m_NBC = 0;
    m_BucketsFirstRung = THRES;
    
    m_bucketwidth = new double[m_NRung];
    m_RCur = new double[m_NRung];
    m_RStart = new double[m_NRung];
    
    // initialise the allocated memory
    memset(m_bucketwidth, 0, sizeof(double) * m_NRung);
    memset(m_RCur, 0, sizeof(double) * m_NRung);
    memset(m_RStart, 0, sizeof(double) * m_NRung);

    m_rungs = new Fifo**[m_NRung];

    for (int i = 0; i < m_NRung; ++i) {
        m_rungs[i] = new Fifo*[THRES];
        for (int j = 0; j < THRES; ++j) {
            m_rungs[i][j] = new Fifo();
        }
    }
}

Ladder::~Ladder()
{
    for (int i = 0; i < m_NRung; ++i) {
        for (int j = 0; j < THRES; ++j) {
            delete m_rungs[i][j];
        }
        delete[] m_rungs[i];
        m_rungs[i] = NULL;
    }
    
    delete[] m_rungs;
    delete[] m_bucketwidth;
    delete[] m_RCur;
    delete[] m_RStart;
}

double Ladder::getBucketwidth(int p_rung) throw (QueueException)
{
    if ((p_rung > m_NRung) || (m_bucketwidth[p_rung] == NULL)) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }

    return m_bucketwidth[p_rung];
}

int Ladder::getNBC() 
{
    return NULL;
}

long Ladder::getNBucket(int p_rung, int p_bucket) 
{
    if (p_rung > m_NRung) {
        throw QueueException(QueueException::RUNG_OUT_OF_BOUNDS);
    }
    if (p_rung == 0) {
        if (p_bucket > m_BucketsFirstRung) {
            throw QueueException(QueueException::BUCKET_OUT_OF_BOUNDS);
        }
    } else {
        if (p_bucket > THRES) {
            throw QueueException(QueueException::BUCKET_OUT_OF_BOUNDS);
        }
    }
    
    return m_rungs[p_rung][p_bucket]->size();
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

double Ladder::bucketwidth(double p_max, double p_min, long p_n) 
{
    return (p_max - p_min) / p_n;
}

double Ladder::bucketwidth(int p_rung) 
{
    return getBucketwidth(p_rung) / THRES;
}

int Ladder::bucket(double p_TS, int p_rung) 
{
    return (int) floor((p_TS - getRStart(p_rung)) / getBucketwidth(p_rung));
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
        int bucket_k = bucket(p_entry->arrival, nRungs);

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

