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
using des::common::entry_t;



namespace des
{
    namespace common
    {


/**
 * This structure specifies a single-linked list element.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
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

/**
 * This structure specifies a double-linked list element.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
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


/**
 * Specification of the <code>List</code> interface.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class List
{
public:
    virtual ~List() = 0;

    virtual void enlist(node_double_t *p_list, long p_size) = 0;
    virtual node_double_t *delist() = 0;
};


    }
}

#endif
