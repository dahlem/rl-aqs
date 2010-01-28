// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file LogGraphHandler.cc
 * Implementation of a basic logGraph handler.
 */
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Entry.hh"
namespace dcommon = des::common;

#include "GraphUtil.hh"
namespace dnet = des::network;

#include "events.hh"
#include "LogGraphHandler.hh"
#include "EventGenerator.hh"


namespace des
{
namespace core
{



LogGraphHandler::LogGraphHandler(std::string p_baseResultDir, dnet::Graph &p_graph,
                                 dcommon::Queue& p_queue, double p_interval, double p_stopTime)
    : m_baseResultDir(p_baseResultDir), m_graph(p_graph), m_queue(p_queue), m_interval(p_interval),
      m_stopTime(p_stopTime), m_counter(0)
{
    std::stringstream path_str;

    path_str << m_baseResultDir << "/" << "graphs";
    m_resultDir = path_str.str();

    // create directory if it doesn't already exist
    fs::path graphs_path(m_resultDir);

    if (!fs::exists(graphs_path)) {
        fs::create_directories(graphs_path);
    }
}


LogGraphHandler::~LogGraphHandler()
{}


void LogGraphHandler::update(AdminEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if (entry->getType() == LOG_GRAPH_EVENT) {
#ifndef NDEBUG_EVENTS
        std::cout << "LogGraphHandler -- graph : " << m_counter << std::endl;
#endif /* NDEBUG_EVENTS */
        std::stringstream file_str;

        file_str << m_resultDir << "/" << "graph" << m_counter << ".gml";
        dnet::GraphUtil::print(m_graph, file_str.str(), dnet::GraphUtil::GRAPHML);

        m_counter++;

#ifndef NDEBUG_EVENTS
        std::cout << "LogGraphHandler -- next time : " << (static_cast<double> (m_counter) * m_interval) << std::endl;
#endif /* NDEBUG_EVENTS */

        if ((static_cast<double> (m_counter) * m_interval) <= m_stopTime) {
#ifndef NDEBUG_EVENTS
            std::cout << "schedule..." << std::endl;
#endif /* NDEBUG_EVENTS */
            EventGenerator::generateLogGraphEvent(m_queue, (static_cast<double> (m_counter) * m_interval));
        }
    }
}

}
}
