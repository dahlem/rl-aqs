// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file RLResponseHandler.hh
 * This header file specifies the ack handler subject.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __CORE_RLRESPONSEHANDLER_HH__
#define __CORE_RLRESPONSEHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "AckEvent.hh"


namespace des
{
namespace core
{

/** @class RLResponseHandler
 * The class @code{RLResponseHandler} handles the RL update statistic.
 */
class RLResponseHandler : public design::Observer<AckEvent>
{
public:
    RLResponseHandler(dnet::tGraphSP p_graph, boost::shared_array<double> p_alpha,
                      boost::shared_array<double> p_r, boost::uint16_t p_levels,
                      double p_q_alpha, double p_q_beta, double p_q_lambda);
    
    ~RLResponseHandler();

    void update(AckEvent *subject);

private:
    dnet::tGraphSP m_graph;
    boost::shared_array<double> m_alpha;
    boost::shared_array<double> m_r;
    boost::uint16_t m_levels;
    double m_q_alpha;
    double m_q_beta;
    double m_q_lambda;
    
    // derived fields
    dnet::VertexVarResponseMap vertex_var_response_map;
    dnet::VertexMeanResponseMap vertex_mean_response_map;
    dnet::VertexNumEventsProcessedMap vertex_num_events_processed_map;
};


/** @typedef tRLResponseHandlerSP
 * a type definition of the shared pointer of the ack handler
 */
typedef boost::shared_ptr <RLResponseHandler> tRLResponseHandlerSP;


}
}


#endif /* __CORE_RLRESPONSEHANDLER_HH__ */
