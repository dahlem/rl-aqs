#include <cstddef>
#include <cfloat>

#include "Top.hh"


Top::Top() 
    : m_maxTS(0.0), m_minTS(DBL_MAX), m_topStart(0.0)
{
    m_fifo = new Fifo();
}

Top::~Top()
{
    delete m_fifo;
}

double Top::getMaxTS()
{
    return m_maxTS;
}

double Top::getMinTS()
{
    return m_minTS;
}

double Top::getTopStart()
{
    return m_topStart;
}

long Top::getNTop()
{
    return m_fifo->size();
}

void Top::reset() throw (QueueException)
{
    if (getNTop() > 0) {
        throw QueueException(QueueException::BAD_RESET);
    } else {
        // do something with m_topStart
        m_maxTS = 0.0;
        m_minTS = DBL_MAX;
    }
}

void Top::enqueue(entry_t *const p_entry)
{
    setMaxTS(p_entry->arrival);
    setMinTS(p_entry->arrival);
    
    m_fifo->enqueue(p_entry);
}

entry_t *const Top::dequeue()
{
    return m_fifo->dequeue();
}

void Top::setMaxTS(double p_maxTS) 
{
    if (p_maxTS > m_maxTS) {
        m_maxTS = p_maxTS;
    }
}

void Top::setMinTS(double p_minTS)
{
    if (p_minTS < m_minTS) {
        m_minTS = p_minTS;
    }
}

node_double_t *Top::delist()
{
    node_double_t *result = NULL;

    try {
        result = m_fifo->delist();
        reset();
    } catch (QueueException &qe) {
        // cannot happen here
    }

    return result;
}

void Top::enlist(node_double_t *p_list, long p_size) 
{
    m_fifo->enlist(p_list, p_size);
}
