// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file OnPolicySelection.hh
 * Declaration of the on-policy selection class
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_ONPOLICYSELECTION_HH__
#define __DES_RL_ONPOLICYSELECTION_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "DesBus.hh"
namespace dcore = des::core;

#include "Selection.hh"


namespace des
{
namespace rl
{

/** @class OnPolicySelection
 * This class selects on-policy actions. This implies that this selection does
 * not need to know which policy is been used, because the action is chosen whenever
 * the new value-function is calculated.
 */
class OnPolicySelection : public Selection
{
public:
    OnPolicySelection(Policy &p_policy, dcore::DesBus&);

    virtual ~OnPolicySelection()
        {}

    boost::int32_t operator() (boost::int32_t p_source);

private:
    dnet::Graph &m_graph;
    dnet::VertexNextActionMap vertex_next_action_map;

};


}
}



#endif /* __DES_RL_ONPOLICYSELECTION_HH__ */
