// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Queue.hh
 * Declaration of the queue interface
 */
#ifndef QUEUE_HH
#define QUEUE_HH

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_LADDERSTATS
# include <ostream>

# include <boost/iostreams/stream.hpp>
# include <boost/iostreams/device/file.hpp>
namespace bio = boost::iostreams;

# include <boost/shared_ptr.hpp>
#endif /* HAVE_LADDERSTATS */

#include "Entry.hh"
#include "QueueException.hh"
namespace dcommon = des::common;



namespace des
{
    namespace common
    {

#ifdef HAVE_LADDERSTATS

/** @typedef str_buf
 * Specifies a shared pointer to the boost iostream stream_buffer
 * in file sink mode.
 */
typedef bio::stream_buffer <bio::file_sink> str_buf;

/** @typedef tOstreamSP
 * Specifies shared pointer to the std::ostream
 */
typedef boost::shared_ptr <std::ostream> tOstreamSP;


/** @typedef tStrBufSP
 * Specifies a shared pointer to the boost::iostreams::stream_buffer
 */
typedef boost::shared_ptr <str_buf> tStrBufSP;

#endif /* HAVE_LADDERSTATS */


/**
 * This class specifies the contract of a queue.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie>Dominik Dahlem</a>
 */
class Queue
{
public:
    virtual ~Queue() = 0;

#ifdef HAVE_LADDERSTATS
    void record();
#endif /* HAVE_LADDERSTATS */

    /**
     * @param entry_t* the entry structure to be enqueued into a queue.
     * @throws QueueException throw, if the queue cannot enqueue the entry.
     */
    virtual void enqueue(dcommon::tEntrySP p_entry) throw (dcommon::QueueException) = 0;

    /**
     * @return returns the next entry in a queue.
     */
    virtual dcommon::tEntrySP dequeue() = 0;

#ifdef HAVE_LADDERSTATS
protected:

    tOstreamSP os;
    tStrBufSP buf;
    int events_out;
    int events_in;

#endif /* HAVE_LADDERSTATS */

};

    }
}

#endif
