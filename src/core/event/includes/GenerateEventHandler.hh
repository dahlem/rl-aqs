// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file UnprocessedEventHandler.hh
 * This header file specifies the unprocessedEvent handler subject.
 */

#ifndef __GENERATEEVENTHANDLER_HH__
#define __GENERATEEVENTHANDLER_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>


#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "LastArrivalEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;

#include "CRN.hh"
namespace dsample = des::sampling;

#include "WEvonet.hh"
namespace dnet = des::network;



namespace des
{
namespace core
{


typedef boost::shared_array <int> tIntSA;
typedef boost::shared_array <boost::int32_t> Int32SA;


/** @class GenerateEventHandler
 * The class @code{GenerateEventHandler} generates events in the DES.
 */
class GenerateEventHandler : public design::Observer<dcore::LastArrivalEvent>
{
public:
    GenerateEventHandler(dnet::Graph&, Int32SA, int, dcommon::Queue&, double);
    ~GenerateEventHandler();

    void update(dcore::LastArrivalEvent *subject);

private:
    dnet::Graph &m_graph;
    Int32SA m_arrivalRngs;
    int m_generations;
    dcommon::Queue &m_queue;
    double m_stopTime;
    tIntSA m_currentGeneration;
    double m_interval;

};



}
}


#endif /* __GENERATEEVENTHANDLER_HH__ */
