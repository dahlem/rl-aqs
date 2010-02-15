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

/** @file Arrivals.cc
 * Implementation of the Arrivals interface.
 */
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_vector.h>

#include "CRN.hh"
#include "MFRW.hh"
#include "Seeds.hh"
namespace dsample = des::sampling;

#include "Arrivals.hh"
#include "CJYArrivals.hh"
#include "ConfigChannel.hh"
#include "GraphChannel.hh"


namespace des {
namespace core {


CJYArrivals::CJYArrivals(DesBus &p_bus) 
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_desArgs(((dynamic_cast<ConfigChannel&> (p_bus.getChannel(id::CONFIG_CHANNEL))).getConfig()))
{
    m_arrivalRates = gsl_matrix_alloc(m_desArgs.net_size, m_desArgs.mfrw_T);
}


CJYArrivals::~CJYArrivals()
{
    gsl_matrix_free(m_arrivalRates);
}


void CJYArrivals::generate()
{
    dnet::VertexArrivalRateMap vertex_arrival_props_map =
        get(vertex_arrival_rate, m_graph);

    for (boost::uint16_t node = 0; node < m_arrivalRates->size1; ++node) {
        boost::uint32_t seedBin = dsample::Seeds::getInstance().getSeed();
        boost::uint32_t seedNorm = dsample::Seeds::getInstance().getSeed();
        boost::uint32_t rngBinIndex = dsample::CRN::getInstance().init(seedBin);
        boost::uint32_t rngNormIndex = dsample::CRN::getInstance().init(seedNorm);
    
        dsample::CRN::getInstance().log(seedBin, "binomial rng for MFRW");
        dsample::CRN::getInstance().log(seedNorm, "normal rng for MFRW");

        dsample::tGslRngSP rngBin = dsample::CRN::getInstance().get(rngBinIndex);
        dsample::tGslRngSP rngNorm = dsample::CRN::getInstance().get(rngNormIndex);

        // get view of node
        gsl_vector_view nodeView = gsl_matrix_row(m_arrivalRates, node);
        double nmax = gsl_sf_log(m_desArgs.mfrw_nmax)/gsl_sf_log(m_desArgs.mfrw_lambda);
        
        dsample::MFRW::path(&nodeView.vector, rngBin, rngNorm,
                            m_desArgs.mfrw_d0, m_desArgs.mfrw_a0, m_desArgs.mfrw_b,
                            m_desArgs.mfrw_lambda, m_desArgs.mfrw_Nc, m_desArgs.mfrw_T,
                            m_desArgs.mfrw_n0, nmax);

        // scale the vector of vertex
        dnet::Vertex vertex = boost::vertex(node, m_graph);
        double arrivalRate = vertex_arrival_props_map[vertex];
        
        // with min
        double min = m_desArgs.mfrw_lower * arrivalRate;

        // and max
        double max = m_desArgs.mfrw_upper * arrivalRate;

        // result: min + v * (max - min)
        double diff = max - min;
        
        gsl_vector_scale(&nodeView.vector, diff);
        gsl_vector_add_constant(&nodeView.vector, min);
    }

}


void CJYArrivals::serialise(boost::uint16_t p_v, boost::uint16_t p_t) throw (MgmtException)
{
    if (p_v >= m_arrivalRates->size1) {
        throw MgmtException(MgmtException::NODE_NOT_EXIST);
    }
    if (p_t >= m_arrivalRates->size2) {
        throw MgmtException(MgmtException::TIME_NOT_EXIST);
    }

    dnet::VertexArrivalRateMap vertex_arrival_props_map =
        get(vertex_arrival_rate, m_graph);

    // serialise the arrival rate for time step p_t into the graph object for vertex p_v
    dnet::Vertex vertex = boost::vertex(p_v, m_graph);
    vertex_arrival_props_map[vertex] = gsl_matrix_get(m_arrivalRates, p_v, p_t);
}




}
}
