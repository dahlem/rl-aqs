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

/** @file GraphUtil.hh
 * Declaration of the utility methods for the @code{WEvonet} class
 *
 * @author Dominik Dahlem
 */
#ifndef __GRAPHUTIL_HH__
#define __GRAPHUTIL_HH__

#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/property_iter_range.hpp>

#include "GraphException.hh"
#include "DirectedGraph.hh"


namespace des { namespace network {

/** @class GraphUtil
 * This class implements utilities to print and read @code{WEvonet} classes.
 */
class GraphUtil
{
public:

    /** @enum GraphTypes
     * This enum declares the supported output graph formats. Currently supported
     * are the dot format (graphviz) and the graphml format.
     */
    enum GraphTypes { GRAPHVIZ = 1, GRAPHML };

    /** @fn void print(const std::string& filename, GraphTypes graphType)
     * Print the graph using either of the supported graph formats. The default format
     * is graphviz.
     *
     * @param const std::string& the filename to be printed into
     * @param const GraphTypes the graph type selected for the output
     */
    static void print(tGraphSP, const std::string& filename, const GraphTypes);

    /** @fn void read(tGraphSP, std::string) throw (GraphException)
     * Read a graph from a file and store it into the first parameter.
     *
     * @param tGraphSP the graph object
     * @param const std::string& the filename to read the graph from
     * @param const GraphTypes the graph type selected for the input
     */
    static void read(tGraphSP, const std::string&, const GraphTypes)
        throw (GraphException);

private:

    GraphUtil()
        {}

    ~GraphUtil()
        {}

    static boost::dynamic_properties getProperties(tGraphSP p_graph);

    /** @fn void print_dot(const std::string& filename)
     * Print the graph using the graphviz interface of BGL into a file
     *
     * @param const std::string& the filename to be printed into
     */
    static void print_dot(tGraphSP, const std::string& filename);

    /** @fn void print_graphml(const std::string& filename)
     * Print the graph into a graphml format
     *
     * @param const std::string& the filename to be printed into
     */
    static void print_graphml(tGraphSP, const std::string& filename);

    /** @fn void read_dot(tGraphSP, const std::string& filename)
     * Read the graph using the graphviz interface of BGL from a file
     *
     * @param tGraphSP the graph object to store the graph
     * @param const std::string& the filename to be printed into
     */
    static void read_dot(tGraphSP, const std::string& filename)
        throw (GraphException);

    /** @fn void read_graphml(tGraphSP, const std::string& filename)
     * Read the graph from a graphml format
     *
     * @param tGraphSP the graph object to store the graph
     * @param const std::string& the filename to be printed into
     */
    static void read_graphml(tGraphSP, const std::string& filename)
        throw (GraphException);
};


    }
}


#endif
