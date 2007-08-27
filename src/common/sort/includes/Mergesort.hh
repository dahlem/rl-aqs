#ifndef MERGESORT_HH
#define MERGESORT_HH


#include "List.hh"



class Mergesort
{
public:
    static node_double_t* sort(node_double_t* p_list);
    static node_double_t* merge(
        node_double_t* p_left,
        node_double_t* p_right);

private:
    Mergesort();
    ~Mergesort();
};


#endif
