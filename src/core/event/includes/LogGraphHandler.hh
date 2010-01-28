// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file LogGraphHandler.hh
 * This header file specifies the logGraph handler subject.
 */

#ifndef __LOGGRAPHHANDLER_HH__
#define __LOGGRAPHHANDLER_HH__

#include <string>


#include "Entry.hh"
namespace dcommon = des::common;

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "AdminEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;


namespace des
{
    namespace core
    {


/** @class LogGraphHandler
 * The class @code{LogGraphHandler} handles log-graph events in the DES.
 */
class LogGraphHandler : public design::Observer<dcore::AdminEvent>
{
public:
    LogGraphHandler(std::string, dnet::Graph&,
                    dcommon::Queue&, double, double);
    ~LogGraphHandler();

    void update(dcore::AdminEvent *subject);

private:
    std::string m_baseResultDir;
    dnet::Graph &m_graph;
    dcommon::Queue &m_queue;
    double m_interval;
    double m_stopTime;
    int m_counter;

    // derived fields
    std::string m_resultDir;

};



    }
}


#endif /* __PROCESSEDEVENTHANDLER_HH__ */
