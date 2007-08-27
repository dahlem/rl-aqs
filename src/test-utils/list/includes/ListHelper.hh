#ifndef FIFO_HH
#define FIFO_HH


#include "List.hh"



class ListHelper
{
public:
    static node_double_t *const link(
        node_double_t *p_left, node_double_t *p_right);
    static void freeResources(node_double_t *p_node);
    

private:
    ListHelper() 
        {}
    ~ListHelper() 
        {}
    
};


#endif
