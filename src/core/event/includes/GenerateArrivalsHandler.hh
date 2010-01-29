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

/** @file UnprocessedEventHandler.hh
 * This header file specifies the unprocessedEvent handler subject.
 */

#ifndef __GENERATEARRIVALSHANDLER_HH__
#define __GENERATEARRIVALSHANDLER_HH__

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

#include "Observer.hh"
namespace design = des::design;

#include "CRN.hh"
namespace dsample = des::sampling;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "LastArrivalEvent.hh"
#include "DesBus.hh"



namespace des
{
namespace core
{


typedef boost::shared_array <boost::int32_t> Int32SA;


/** @class GenerateArrivalsHandler
 * The class @code{GenerateArrivalsHandler} generates events in the DES.
 */
class GenerateArrivalsHandler : public design::Observer<dcore::LastArrivalEvent>
{
public:
    GenerateArrivalsHandler(DesBus&, Int32SA);
    ~GenerateArrivalsHandler();

    void update(dcore::LastArrivalEvent *subject);

private:
    dnet::Graph &m_graph;
    dcommon::Queue &m_queue;
    Int32SA m_arrivalRngs;

};



}
}


#endif /* __GENERATEARRIVALSHANDLER_HH__ */
