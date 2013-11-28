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

/** @file DesBus.cc
 * Implementation of the discrete event simulator bus interface
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "DesBus.hh"



namespace des
{
namespace core
{

DesBus::DesBus() 
{
}

DesBus::~DesBus() 
{
}

void DesBus::addChannel(Channel &p_channel) 
{
    m_channels.push_back(&p_channel);
}


Channel& DesBus::getChannel(boost::uint16_t p_id) throw (MgmtException)
{
    if (p_id >= m_channels.size()) {
        throw MgmtException(MgmtException::CHANNEL_NOT_EXIST);
    } else {
        return *(m_channels[p_id]);
    }
}





}
}
