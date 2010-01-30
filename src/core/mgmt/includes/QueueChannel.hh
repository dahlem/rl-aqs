// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file QueueChannel.hh
 * Specification of the QueueChannel interface.
 */
#ifndef __DES_CORE_MGMT_QUEUECHANNEL_HH__
#define __DES_CORE_MGMT_QUEUECHANNEL_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "Channel.hh"


namespace des {
namespace core {


class QueueChannel : public Channel
{
public:
    QueueChannel(dcommon::Queue &p_queue) : m_queue(p_queue) {};
    virtual ~QueueChannel() {};

    virtual boost::uint16_t getId()
        { return id::QUEUE_CHANNEL; }

    dcommon::Queue& getQueue() 
        {
            return m_queue;
        }
    
    
private:
    dcommon::Queue &m_queue;
    
};



}
}


#endif