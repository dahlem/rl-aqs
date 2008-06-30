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

/** @file epidemic_visit.hh
 * Declaration of the methods for the epidemic visit templates.
 *
 * @author Dominik Dahlem
 */
#ifndef __EPIDEMIC_VISIT_HH__
#define __EPIDEMIC_VISIT_HH__

#include <boost/config.hpp>
#include <boost/pending/queue.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/named_function_params.hpp>


namespace boost
{

    template <class IncidenceGraph, class Buffer, class BFSVisitor>
    void epidemic_visit(
        const IncidenceGraph& g,
        typename graph_traits <IncidenceGraph>::vertex_descriptor s,
        Buffer& Q, BFSVisitor vis)
    {
        function_requires <IncidenceGraphConcept<IncidenceGraph> >();
        typedef graph_traits <IncidenceGraph> GTraits;
        typedef typename GTraits::vertex_descriptor Vertex;
        typedef typename GTraits::edge_descriptor Edge;
        function_requires <BFSVisitorConcept <BFSVisitor, IncidenceGraph> >();
        typename GTraits::out_edge_iterator ei, ei_end;

                                                      vis.discover_vertex(s, g);
        Q.push(s);
        while (! Q.empty()) {
            Vertex u = Q.top(); Q.pop();              vis.examine_vertex(u, g);
            for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
                Vertex v = target(*ei, g);            vis.examine_edge(*ei, g);
                                                      vis.discover_vertex(u, g);
                Q.push(v);
            }
                                                      vis.finish_vertex(u, g);
        }
    }


    template <class IncidenceGraph, class P, class T>
    void epidemic_visit(
        const IncidenceGraph& g,
        typename graph_traits <IncidenceGraph>::vertex_descriptor s,
        const bgl_named_params <P, T>& params)
    {
        // The graph is passed by *const* reference so that graph adaptors
        // (temporaries) can be passed into this function. However, the
        // graph is not really const since we may write to property maps
        // of the graph.
        IncidenceGraph& ng = const_cast <IncidenceGraph&>(g);

        typedef graph_traits <IncidenceGraph> Traits;
        // Buffer default
        typedef typename Traits::vertex_descriptor vertex_descriptor;
        typedef boost::queue <vertex_descriptor> queue_t;
        queue_t Q;
        detail::wrap_ref <queue_t> Qref(Q);

        epidemic_visit
            (ng, s,
             choose_param(get_param(params, buffer_param_t()),
                          Qref).ref,
             choose_param(get_param(params, graph_visitor),
                          make_bfs_visitor(null_visitor())));
    }
}


#endif
