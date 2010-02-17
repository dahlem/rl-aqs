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

/** @file GenerateArrivalsHandler.hh
 * This header file specifies the generate arrivals handler subject.
 */

#ifndef __CL_CORE_ROLLINGSYSTEMSTATSADMINHANDLER_HH__
#define __CL_CORE_ROLLINGSYSTEMSTATSADMINHANDLER_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "OnlineStats.hh"
namespace dstats = des::statistics;

#include "clconfig.hh"
#include "AdminEvent.hh"
#include "DesBus.hh"



namespace des
{
namespace core
{


/** @class RollingSystemStatsAdminHandler
 * The class @code{RollingSystemStatsAdminHandler} generates arrival events in the DES.
 */
class RollingSystemStatsAdminHandler : public design::Observer<dcore::AdminEvent>
{
public:
    RollingSystemStatsAdminHandler(DesBus&);
    ~RollingSystemStatsAdminHandler();

    void update(dcore::AdminEvent *subject);

private:
    dnet::Graph &m_graph;
    dcommon::Queue &m_queue;
    desArgs_t &m_desArgs;
    dstats::OnlineStats *m_delay;
    dstats::OnlineStats *m_numEvents;
    dstats::OnlineStats *m_processingTime;
    
};



}
}


#endif /* __CL_CORE_ROLLINGSYSTEMSTATSADMINHANDLER_HH__ */
