// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file QueueManager.hh
 * Declaration of the common random number interface.
 */
#ifndef __QUEUEMANAGER_HH__
#define __QUEUEMANAGER_HH__

#include <boost/shared_ptr.hpp>

#include "Singleton.hh"
namespace ddesign = des::design;

#include "LadderQueue.hh"
namespace dcommon = des::common;


namespace des
{
    namespace core
    {


/**
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class QueueManager : public ddesign::Singleton <QueueManager>
{
public:

    /** @fn
     */
    void set(dcommon::tQueueWP queue);
    dcommon::tQueueWP get();

    QueueManager();
    ~QueueManager();

private:

    dcommon::tQueueWP m_queue;

};


    }
}

#endif
