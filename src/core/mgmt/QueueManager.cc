// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file QueueManager.cc
 * Implementation of the queue manager interface.
 */

#include "QueueManager.hh"
namespace dcore = des::core;

#include "LadderQueue.hh"
namespace dcommon = des::common;



dcore::QueueManager::QueueManager()
{}


dcore::QueueManager::~QueueManager()
{}


void dcore::QueueManager::set(dcommon::tQueueWP queue)
{
    m_queue = queue;
}

dcommon::tQueueWP dcore::QueueManager::get()
{
    return m_queue;
}
