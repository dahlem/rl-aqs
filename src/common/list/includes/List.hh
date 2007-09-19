// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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
