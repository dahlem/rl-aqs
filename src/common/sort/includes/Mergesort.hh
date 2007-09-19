// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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
