// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file Entry.hh
 * Specification of an entry for the priority queue.
 */
#ifndef __ENTRY_HH__
#define __ENTRY_HH__


#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive/list.hpp>



namespace des
{
    namespace common
    {

static const std::string HEADER = "uid,id,arrivalTime,origin,destination,type";


/** @class Entry
 * This structure specifies an (event) entry for the priority queue.
 * An event is specified by the arrival time, the destination, the origin, and
 * a type of the event.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Entry : public boost::intrusive::list_base_hook<>
{
public:
    Entry()
        : id(0), arrival(0.0), destination(-99), origin(-99), type(-99)
        {}

    explicit Entry(double a, int d, int o, int t)
        : arrival(a), destination(d), origin(o), type(t)
        {
            uid++;
            id = uid;
        }

    explicit Entry(boost::uintmax_t i, double a, int d, int o, int t)
        : id(i), arrival(a), destination(d), origin(o), type(t)
        {
            uid++;
        }

    static std::string header()
        {
            return HEADER;
        }

    bool operator< (const Entry& rhs) const;
    bool operator< (const Entry& rhs);


    friend std::ostream& operator <<(std::ostream &p_os, const Entry &p_entry);
    friend std::ostream& operator <<(std::ostream &p_os, Entry &p_entry);


    boost::uintmax_t id;
    double arrival;
    int destination;
    int origin;
    int type;


private:
    static boost::uintmax_t uid;

};

typedef boost::intrusive::make_list <Entry>::type EntryList;
typedef boost::shared_array <EntryList> EntryListSA;
typedef boost::shared_array <EntryListSA> EntryListSM;

typedef EntryList::iterator EntryListIterator;


//The disposer object function
struct delete_disposer
{
    void operator()(Entry *delete_this)
        {
            delete delete_this;
        }
};


    }
}



#endif
