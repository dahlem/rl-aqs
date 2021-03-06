// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
namespace dcommon = des::common;



namespace des
{
    namespace common
    {


/**
 * This class provides two methods to sort and merge a double-linked list
 * with the mergesort algorithm invented by John von Neumann. The characteristics
 * are its stability, \f$ O(N \log N) \f$ average and worst-case time complexity,
 * and \f$ O(N) \f$ memory complexity.
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class Mergesort
{
public:
    /**
     * Recursive operation in order to sort a given list according to the
     * arrival timestamp of the events.
     *
     * @param node_double_t* the list to be sorted
     * @return the sorted list
     */
    static dcommon::node_double_t* sort(dcommon::node_double_t* p_list);

    /**
     * Merge operation of two sorted lists.
     *
     * @param node_double_t* sorted list
     * @param node_double_t* sorted list
     */
    static dcommon::node_double_t* merge(
        dcommon::node_double_t* p_left,
        dcommon::node_double_t* p_right);

private:
    /**
     * Hide the constructor, because this class is a utility class without
     * internal state.
     */
    Mergesort();
    ~Mergesort();
};


    }
}

#endif
