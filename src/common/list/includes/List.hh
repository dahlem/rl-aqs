// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file List.hh
 * Defines structures for single-/double-linked lists.
 */
#ifndef LIST_HH
#define LIST_HH


#include "Entry.hh"
namespace dcommon = des::common;



namespace des
{
    namespace common
    {


/** @struct node_single_t
 * This structure specifies a single-linked list element.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
struct node_single_t
{
    tEntrySP data;
    node_single_t *next;

    node_single_t(tEntrySP d,
                  node_single_t *n)
        {
            data = d;
            next = n;
        }

};

/** @struct node_double_t
 * This structure specifies a double-linked list element.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
struct node_double_t
{
    tEntrySP data;
    node_double_t *next;
    node_double_t *previous;

    node_double_t(tEntrySP d,
                  node_double_t *n,
                  node_double_t *p)
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

    /** @fn enlist(node_double_t *p_list, long p_size)
     * Enlist double-linked list elements
     *
     * @param node_double_t* the partial list to be enlisted
     * @param long the size of the partial list
     */
    virtual void enlist(node_double_t *p_list, long p_size) = 0;

    /** @fn node_double_t *delist()
     * Delist one element from the double-linked list
     *
     * @return the list element
     */
    virtual node_double_t *delist() = 0;
};


    }
}

#endif
