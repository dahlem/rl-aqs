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

/** @file StaticArrivalsChannel.hh
 * Specification of the StaticArrivalsChannel interface.
 */
#ifndef __DES_CORE_MGMT_STATICARRIVALSCHANNEL_HH__
#define __DES_CORE_MGMT_STATICARRIVALSCHANNEL_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif


#include "ArrivalsChannel.hh"


namespace des {
namespace core {


class StaticArrivalsChannel : public ArrivalsChannel
{
public:
    StaticArrivalsChannel() {};
    virtual ~StaticArrivalsChannel() {};

};



}
}


#endif