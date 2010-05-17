// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file CoefficientVariationHandler.hh
 * This header file specifies the ack handler subject for the
 * coefficient of variation calculation.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __CORE_COEFFICIENTVARIATIONHANDLER_HH__
#define __CORE_COEFFICIENTVARIATIONHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/circular_buffer.hpp>
#include <boost/cstdint.hpp>
#include <boost/scoped_array.hpp>


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
namespace drl = des::rl;

#include "AckEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{

typedef boost::circular_buffer<double> CircularBuffer;


/** @class CoefficientVariationHandler
 * The class @code{CoefficientVariationHandler} handles the coefficient of
 * variation metric. The position of this handler has to be behind the RL handlers, because
 * it depends on the calculation of the Q-values.
 */
class CoefficientVariationHandler : public design::Observer<AckEvent>
{
public:
    CoefficientVariationHandler(DesBus&);
    ~CoefficientVariationHandler();

    void update(AckEvent *subject);

private:
    dnet::Graph &m_graph;
    CircularBuffer *m_circBuffer;

    // derived fields
    dnet::EdgeIndexMap edge_index_map;
    dnet::EdgeQValueMap edge_q_val_map;
    dnet::VertexCoeffVarMap vertex_coeff_var_map;
};


}
}


#endif /* __CORE_CoefficientVariationHandler_HH__ */
