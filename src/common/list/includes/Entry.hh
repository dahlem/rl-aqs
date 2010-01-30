// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#if NDEBUG
# include <boost/assert.hpp>
#endif

#include <iostream>
#include <stack>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/intrusive/list.hpp>

//#include "Stack.hh"


namespace des
{
namespace common
{

// typedef Stack <int> StackInt;
// typedef Stack <double> StackDouble;
typedef std::stack <int> StackInt;
typedef std::stack <double> StackDouble;


static const std::string HEADER = "uid,id,arrivalTime,delay,origin,destination,type,eventPathSize";


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
    Entry();

    explicit Entry(double del, double a, int d, int o, int t);

    explicit Entry(const Entry &p_entry);

    ~Entry();

    static std::string header()
        {
            return HEADER;
        }

    bool operator< (const Entry& rhs) const;
    bool operator< (const Entry& rhs);

    void delayed(double, double, boost::int32_t);
    void service(double, boost::int32_t);
    void depart(boost::int32_t, boost::int32_t);
    void acknowledge(boost::int32_t, boost::int32_t, boost::int32_t);
    void leave(boost::int32_t, boost::int32_t);

    double getDelay() const;

    boost::uintmax_t getId() const;

    double getArrival() const;
    void setArrival(double p_arrival);

    int getDestination() const;

    int getOrigin() const;

    int getType() const;

    bool isEventQueueEmpty();
    int popEvent();

    double topArrival();
    double getExternalArrival();
    double getReward();

    void pushEventHistory(boost::int32_t, double);
    int getEventHistoryLength();

    friend std::ostream& operator <<(std::ostream &p_os, const Entry &p_entry)
        {
            p_os << p_entry.gid << "," << p_entry.id << "," << p_entry.arrival << ","
                 << p_entry.delay << "," << p_entry.origin << "," << p_entry.destination << ","
                 << p_entry.type << "," << p_entry.event_path.size();

            return p_os;
        }



private:
    Entry& operator=(const Entry&) { return *this; }

    static boost::uintmax_t uid;
    boost::uintmax_t id;
    double delay;
    double arrival;
    double externalArrival;
    int destination;
    int origin;
    int type;
    StackInt event_path;
    StackDouble event_arrivals;
    boost::uintmax_t gid;

};

typedef boost::intrusive::make_list <Entry, boost::intrusive::link_mode<boost::intrusive::auto_unlink> >::type EntryList;
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
