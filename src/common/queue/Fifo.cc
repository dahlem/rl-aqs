#include <cstddef>

#include "Fifo.hh"


Fifo::Fifo() 
{
    init();
}


Fifo::~Fifo()
{
    node_double_t *current = m_head->next;
    
    while (current) {
        // if tail node
        if (current->data == NULL) break;

        node_double_t *deleteNode = current;

        // advance the current pointer
        current = current->next;

        // delete and the node
        delete deleteNode;
    }
    
    delete m_head;
    delete m_tail;
}

void Fifo::init() 
{
    m_head = new node_double_t(NULL, NULL, NULL);
    m_tail = new node_double_t(NULL, NULL, NULL);

    m_head->next = m_tail;
    m_tail->previous = m_head;

    m_size = 0;
}

const long Fifo::size()
{
    return m_size;
}

void Fifo::enqueue(entry_t *const p_entry)
{
    node_double_t *node = new node_double_t(p_entry, m_tail, m_tail->previous);
    
    m_tail->previous->next = node;
    m_tail->previous = node;
    
    m_size++;
}

entry_t *const Fifo::dequeue()
{
    node_double_t *temp = m_head->next;
    entry_t *const result = temp->data;

    if (m_size == 0) {
        return NULL;
    }
    
    m_head->next = temp->next;
    temp->next->previous = m_head;

    temp->next = NULL;
    temp->previous = NULL;
    
    delete temp;
    m_size--;
    
    return result;
}

node_double_t *Fifo::getList()
{
    node_double_t *list = m_head;

    // re-initialise the fifo data structure
    init();

    // return the current list
    return list;
}
