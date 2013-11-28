// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file RandomSelection.hh
 * Declaration of an abstract randomSelection class
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_RANDOMSELECTION_HH__
#define __DES_RL_RANDOMSELECTION_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "DesBus.hh"
namespace dcore = des::core;

#include "CRN.hh"
namespace dsample = des::sampling;

#include "Selection.hh"


namespace des
{
namespace rl
{

typedef boost::shared_array <boost::int32_t> Int32SA;


class RandomSelection : public Selection
{
public:
    RandomSelection(dcore::DesBus&, Policy&, Int32SA);

    virtual ~RandomSelection()
        {}

    boost::int32_t operator() (boost::int32_t p_source, PAttr p_attr);

private:
    dnet::Graph &m_graph;
    Int32SA m_depart_uniform_ids;
    dnet::EdgeWeightMap edge_weight_map;
    dnet::VertexIndexMap vertex_index_map;

};


}
}



#endif /* __DES_RL_RANDOMSELECTION_HH__ */
