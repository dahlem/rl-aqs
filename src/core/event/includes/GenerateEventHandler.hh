// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#include <boost/shared_ptr.hpp>


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


/** @class GenerateEventHandler
 * The class @code{GenerateEventHandler} generates events in the DES.
 */
class GenerateEventHandler : public design::Observer<dcore::LastArrivalEvent>
{
public:
    GenerateEventHandler(dnet::tGraphSP, dsample::tGslRngSP, int, dcommon::tQueueSP, double);
    ~GenerateEventHandler();

    void update(dcore::LastArrivalEvent *subject);

private:
    dnet::tGraphSP m_graph;
    dsample::tGslRngSP m_arrivalRng;
    int m_generations;
    dcommon::tQueueSP m_queue;
    double m_stopTime;
    tIntSA m_currentGeneration;
    double m_interval;

};


/** @typedef tGenerateEventHandlerSP
 * a type definition of the shared pointer of the GenerateEvent handler
 */
typedef boost::shared_ptr <GenerateEventHandler> tGenerateEventHandlerSP;


    }
}


#endif /* __GENERATEEVENTHANDLER_HH__ */
