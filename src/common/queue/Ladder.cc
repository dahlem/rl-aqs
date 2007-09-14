#include <cmath>
#include <cstddef>
#include <cstring>

#include "Ladder.hh"



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
    m_BucketsFirstRung = DEFAULT_THRES;

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
        m_rungs[i] = new Fifo*[DEFAULT_THRES];
        for (int j = 0; j < DEFAULT_THRES; ++j) {
            m_rungs[i][j] = new Fifo();
        }
    }
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

long Ladder::getNBucket(int p_rung, int p_bucket)
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

void Ladder::enqueue(entry_t *const p_entry) throw (QueueException)
{
    int nRungs = 0;

    // find the rung
    while ((p_entry->arrival < getRCur(nRungs)) && (nRungs <= getNRung())) {
        nRungs++;
    }

    // found
    if (nRungs <= getNRung()) {
//        int bucket_k = bucket(p_entry->arrival, nRungs);

        // insert into tail of rung x, bucket k
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
{
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

    // advance the current dequeue bucket
    advanceDequeueBucket(1);
}

node_double_t *Ladder::delist()
{
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

    // skip empty tail buckets of the current rung
    if (m_events[m_lowestRung] == 0) {
        if (m_lowestRung > 0) {
            m_lowestRung--;
        }
    }

    // find next non-empty bucket from lowest rung
    while (true) {
        // leave, if the current bucket cannot be advanced further
        // two cases: (1) we are on the first rung; (2) we are between 0-m_NRung
        if (m_lowestRung == 0) {
            if (m_currentBucket[m_lowestRung] == (m_BucketsFirstRung - 1)) {
                break;
            }
        } else {
            if (m_lowestRung < m_NRung) {
                if (m_currentBucket[m_lowestRung] == (m_Thres - 1)) {
                    break;
                }
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
        advanceDequeueBucket(spawn(elements));
    }
}

bool Ladder::spawn(long p_elements)
{
    bool result = false;    

    // spawn, iff
    // (1) lowest rung < MAX_RUNGS - 1
    // (2) m_events[i] > m_rungs[i][j]
    if ((m_events[m_lowestRung]
         > m_rungs[m_lowestRung][m_currentBucket[m_lowestRung]]->size())
        && (m_lowestRung < (MAX_RUNGS - 1))) {
        m_lowestRung++;

        // create new rung, iff
        // (1) m_NRung < MAX_RUNGS
        if ((m_lowestRung >= m_NRung) && (m_NRung < MAX_RUNGS)) {
            createRung();
        }

        m_bucketwidth[m_lowestRung] =
            getBucketwidth(m_lowestRung - 1) / (m_Thres  - 1);
        m_RStart[m_lowestRung] =
            getRCur(m_lowestRung - 1)
            - getBucketwidth(m_lowestRung - 1);
        m_RCur[m_lowestRung] =
            getRCur(m_lowestRung - 1)
            - getBucketwidth(m_lowestRung - 1)
            + getBucketwidth(m_lowestRung);

        // copy the elements from the previous bucket to the next rung
        int size = m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]->size();
        m_events[m_lowestRung - 1] -= size;
        
        enlist(
            m_lowestRung,
            m_rungs[m_lowestRung - 1][m_currentBucket[m_lowestRung - 1]]->delist()->next,
            size);

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
}
