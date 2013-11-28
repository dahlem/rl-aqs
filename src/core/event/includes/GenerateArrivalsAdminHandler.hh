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

/** @file GenerateArrivalsHandler.hh
 * This header file specifies the generate arrivals handler subject.
 */

#ifndef __GENERATEARRIVALSADMINHANDLER_HH__
#define __GENERATEARRIVALSADMINHANDLER_HH__

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

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "AdminEvent.hh"
#include "DesBus.hh"



namespace des
{
namespace core
{

typedef boost::shared_array <boost::int32_t> Int32SA;


/** @class GenerateArrivalsAdminHandler
 * The class @code{GenerateArrivalsAdminHandler} generates arrival events in the DES.
 */
class GenerateArrivalsAdminHandler : public design::Observer<dcore::AdminEvent>
{
public:
    GenerateArrivalsAdminHandler(DesBus&, Int32SA);
    ~GenerateArrivalsAdminHandler();

    void update(dcore::AdminEvent *subject);

private:
    Int32SA m_arrivalRngs;
    dnet::Graph &m_graph;
    dcommon::Queue &m_queue;

};



}
}


#endif /* __GENERATEARRIVALSADMINHANDLER_HH__ */
