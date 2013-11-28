// Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
#include <vector>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphml.hpp>

#include "IntSet.hh"

struct City
{
    std::string name;
    int population;
    des::network::IntSet zipcodes;
    // std::vector<int> zipcodes;
};


struct Highway
{
    std::string name;
    double miles;
    int speed_limit;
    int lanes;
    bool divided;
};

int main()
{
    typedef boost::adjacency_list<
        boost::listS, boost::vecS, boost::bidirectionalS,
        City, Highway> Graph;

    Graph g;
    Graph::vertex_descriptor v1 = boost::add_vertex(g);
    Graph::vertex_descriptor v2 = boost::add_vertex(g);
    boost::add_edge(v1, v2, g);

    Graph::vertex_descriptor v = *vertices(g).first;
    g[v].name = "Troy";
    g[v].population = 49170;
    g[v].zipcodes.insert(12180);
    g[v].zipcodes.insert(12181);
    Graph::edge_descriptor e = *out_edges(v, g).first;
    g[e].name = "I-87";
    g[e].miles = 10;
    g[e].speed_limit = 65;
    g[e].lanes = 4;
    g[e].divided = true;

    boost::dynamic_properties dp;
    dp.property("zip_codes", boost::get(&City::zipcodes, g));
    dp.property("name", boost::get(&City::name, g));

    std::ofstream out("test.gml", std::ios::out);
    boost::write_graphml(out, g, boost::get(boost::vertex_index, g), dp, false);
    out.close();

    Graph g2;
    std::ifstream in("test.gml", std::ifstream::in);
    std::cout << "Reading graph: test.gml" << std::endl;
    boost::read_graphml(in, g2, dp);
    v = *vertices(g2).first;

    std::cout << "name: " << g[v].name << ", population: " << g[v].population
              << ", # zipcodes: " << g[v].zipcodes.size() << ", zipcodes: ";

    for (std::set<int>::iterator iter = g[v].zipcodes.begin(); iter != g[v].zipcodes.end(); ++iter) {
        std::cout << *iter << ",";
    }

    g[v].zipcodes.erase(g[v].zipcodes.begin(), g[v].zipcodes.end());
    std::cout << std::endl << "size after deletion: " << g[v].zipcodes.size() << std::endl;

    std::cout << std::endl;

    return EXIT_SUCCESS;
}
