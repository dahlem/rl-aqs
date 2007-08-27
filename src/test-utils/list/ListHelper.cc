#include <cstdlib>

#include "ListHelper.hh"


node_double_t *const ListHelper::link(
    node_double_t *p_left, node_double_t *p_right) 
{
    p_left->next = p_right;
    p_right->previous = p_left;

    return p_left;
}
