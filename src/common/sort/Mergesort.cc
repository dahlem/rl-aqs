// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <cstddef>

#include <boost/shared_ptr.hpp>

#include "Entry.hh"
#include "Mergesort.hh"
namespace dcommon = des::common;



dcommon::Mergesort::Mergesort()
{}

dcommon::Mergesort::~Mergesort()
{}

dcommon::node_double_t* dcommon::Mergesort::sort(
    dcommon::node_double_t* p_list)
{
    if (p_list == NULL || p_list->next == NULL || p_list->next->data == NULL)
        return p_list;

    dcommon::node_double_t *left = p_list;
    dcommon::node_double_t *right = p_list->next;

    while ((right != NULL)
           && (right->next != NULL)
           && (right->next->next != NULL)) {
        p_list = p_list->next;
        right = right->next->next;
    }

    right = p_list->next;
    p_list->next = NULL;


    return merge(sort(left), sort(right));
}

dcommon::node_double_t* dcommon::Mergesort::merge(
    dcommon::node_double_t* p_left,
    dcommon::node_double_t* p_right)
{
    dcommon::entry_t *entry = NULL;
    dcommon::node_double_t dummy(dcommon::tEntrySP(entry), NULL, NULL);
    dcommon::node_double_t *head = &dummy;
    dcommon::node_double_t *temp = head;

    while ((p_left != NULL) && (p_right != NULL)) {
        if ((p_left->data == NULL) && (p_right->data == NULL)) break;

        if (p_left->data == NULL) {
            p_right->previous = temp;
            temp->next = p_right;
            temp = p_right;
            p_right = p_right->next;
        } else if (p_right->data == NULL) {
            p_left->previous = temp;
            temp->next = p_left;
            temp = p_left;
            p_left = p_left->next;
        } else if (p_left->data->arrival <= p_right->data->arrival) {
            p_left->previous = temp;
            temp->next = p_left;
            temp = p_left;
            p_left = p_left->next;
        } else {
            p_right->previous = temp;
            temp->next = p_right;
            temp = p_right;
            p_right = p_right->next;
        }
    }

    temp->next = (p_left == NULL) ? p_right : p_left;
    temp->next->previous = temp;

    return head->next;
}
