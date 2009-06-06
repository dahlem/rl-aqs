// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file RLStateHelper.hh
 * Helper methods to retrieve RL State information
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_RLSTATEHELPER_HH__
#define __DES_RL_RLSTATEHELPER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */


#include <vector>

#include <boost/shared_array.hpp>

#include "DirectedGraph.hh"
namespace dnet = des::network;


namespace des
{
namespace rl
{

typedef boost::shared_array <double> DoubleSA;


class RLStateHelper
{
public:
    static void fillStateVector(dnet::Edge &p_edge,
                                dnet::Graph &p_graph,
                                std::vector<int> &p_stateRepresentation,
                                DoubleSA p_inputs);
    

private:
    RLStateHelper()
        {}
    ~RLStateHelper()
        {}

};


}
}



#endif /* __DES_RL_RLSTATEHELPER_HH__ */
