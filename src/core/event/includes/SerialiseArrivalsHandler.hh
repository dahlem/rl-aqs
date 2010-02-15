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

/** @file SerialiseArrivalsHandler.hh
 * This header file specifies the serialise arrivals handler subject.
 */

#ifndef __SERIALISEARRIVALSHANDLER_HH__
#define __SERIALISEARRIVALSHANDLER_HH__

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

#include "clconfig.hh"
#include "AdminEvent.hh"
#include "Arrivals.hh"
#include "DesBus.hh"



namespace des
{
namespace core
{


/** @class SerialiseArrivalsHandler
 * The class @code{SerialiseArrivalsHandler} serialises arrival events in the DES.
 */
class SerialiseArrivalsHandler : public design::Observer<dcore::AdminEvent>
{
public:
    SerialiseArrivalsHandler(DesBus&);
    ~SerialiseArrivalsHandler();

    void update(dcore::AdminEvent *subject);

private:
    dcommon::Queue &m_queue;
    desArgs_t &m_desArgs;
    Arrivals *m_arrivals;
    boost::uint16_t *m_currentTimeStep;
    double m_interval;

};



}
}


#endif /* __SERIALISEARRIVALSHANDLER_HH__ */
