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

/** @file ArrivalsChannel.hh
 * Specification of the ArrivalsChannel interface.
 */
#ifndef __DES_CORE_MGMT_ARRIVALSCHANNEL_HH__
#define __DES_CORE_MGMT_ARRIVALSCHANNEL_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>


namespace des {
namespace core {


class ArrivalsChannel
{
public:
    ArrivalsChannel() {};
    virtual ~ArrivalsChannel() {};

    virtual boost::uint16_t getId() 
        { return id::ARRIVAL_CHANNEL; }

};



}
}


#endif
