// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LISTHELPER_HH
#define LISTHELPER_HH


#include "List.hh"
using des::common::node_double_t;


namespace des
{
    namespace testutils
    {
        
/**
 * This class does not have an internal state, but provides static helper
 * methods for the test cases to link and free up <code>node_double_t</code>s.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class ListHelper
{
public:
    /**
     * Link two nodes together.
     *
     * @param node_double_t* a pointer to the left node
     * @param node_double_t* a pointer to the right node
     * @return returns the node pointer to the left node
     */
    static node_double_t *const link(
        node_double_t *p_left, node_double_t *p_right);

    /**
     * Free the resources of a <code>node_double_t*</code>
     *
     * @param node_double_t* a pointer to be freed
     */
    static void freeResources(node_double_t *p_node);
    

private:
    /**
     * Prevent the creation of objects of this type
     */
    ListHelper() 
        {}

    /**
     * Prevent the destruction of objects of this type
     */
    ~ListHelper() 
        {}
    
};

    }
}

#endif
