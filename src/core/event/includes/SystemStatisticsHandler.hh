// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file SystemStatisticsHandler.hh
 * This header file specifies the system statistics handler subject.
 */

#ifndef __DES_CORE_SYSTEMSTATISTICSHANDLER_HH__
#define __DES_CORE_SYSTEMSTATISTICSHANDLER_HH__


#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Observer.hh"
namespace design = des::design;

#include "PostEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{


/** @class SystemStatisticsHandler
 * The class @code{SystemStatisticsHandler} handles system statistics events in the DES.
 */
class SystemStatisticsHandler : public design::Observer<PostEvent>
{
public:
    SystemStatisticsHandler(DesBus&);
    ~SystemStatisticsHandler();

    void update(PostEvent *subject);

private:
    dnet::Graph &m_graph;

};



}
}


#endif /* __DES_CORE_SYSTEMSTATISTICSHANDLER_HH__ */
