// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file ProcessedEventHandler.hh
 * This header file specifies the processedEvent handler subject.
 */

#ifndef __PROCESSEDEVENTHANDLER_HH__
#define __PROCESSEDEVENTHANDLER_HH__

#include "PreAnyEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;

#include "Results.hh"
namespace dio = des::io;


namespace des
{
    namespace core
    {


/** @class ProcessedEventHandler
 * The class @code{ProcessedEventHandler} handles processedEvent events in the DES.
 */
class ProcessedEventsHandler : public design::Observer<dcore::PreAnyEvent>
{
public:
    ProcessedEventsHandler(dio::Results&);
    ~ProcessedEventsHandler();

    void update(dcore::PreAnyEvent *subject);

private:
    dio::Results &m_processedEvents;

};



    }
}


#endif /* __PROCESSEDEVENTHANDLER_HH__ */
