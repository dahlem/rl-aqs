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


#endif
