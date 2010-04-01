// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file CognitiveValueHandler.hh
 * This header file specifies the ack handler subject.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __CORE_COGNITIVEVALUEHANDLER_HH__
#define __CORE_COGNITIVEVALUEHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/scoped_array.hpp>


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "AckEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{

typedef boost::scoped_array <double> tDoubleSA;

/** @class CognitiveValueHandler
 * The class @code{CognitiveValueHandler} handles the absolute expert metrics.
 */
class CognitiveValueHandler : public design::Observer<AckEvent>
{
public:
    CognitiveValueHandler(DesBus&);
    ~CognitiveValueHandler();

    void update(AckEvent *subject);

private:
    dnet::Graph &m_graph;
    tDoubleSA m_lastTime;

    // derived fields
    double m_ANeg;
    double m_APos;
    double m_rNeg;
    double m_rPos;

    dnet::EdgeEmotionMap edge_emotion_map;
    dnet::EdgeRMinMap edge_rmin_map;
    dnet::EdgeRMaxMap edge_rmax_map;
    dnet::EdgeEPosMap edge_e_pos_map;
    dnet::EdgeENegMap edge_e_neg_map;
    dnet::EdgeIndexMap edge_index_map;
    dnet::EdgeQValueMap edge_q_val_map;
};


}
}


#endif /* __CORE_COGNITIVEVALUEHANDLER_HH__ */
