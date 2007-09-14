#ifndef LIST_HH
#define LIST_HH


#include "Entry.hh"



struct node_single_t 
{
    struct entry_t *data;
    struct node_single_t *next;

    node_single_t(struct entry_t *const d,
                  struct node_single_t *n) 
        {
            data = d;
            next = n;
        }
    
};

struct node_double_t 
{
    struct entry_t *data;
    struct node_double_t *next;
    struct node_double_t *previous;

    node_double_t(struct entry_t *const d,
                  struct node_double_t *n,
                  struct node_double_t *p) 
        {
            data = d;
            next = n;
            previous = p;
        }
    
};


class List
{
public:
    virtual ~List() = 0;

    virtual void enlist(node_double_t *p_list, long p_size) = 0;
    virtual node_double_t *delist() = 0;
};


#endif
