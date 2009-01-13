#!/bin/sh
# networks_1000.sh
# generate random and social graphs
#
# Usage: ./networks_1000.sh <boost_vertex> <boost_edge>

USAGE="./networks_1000.sh <boost_vertex> <boost_edge>";
BOOST_ARRIVAL=$1;
BOOST_EDGES=$2;

if [ -z "$BOOST_ARRIVAL" ] ; then
echo "WARNING: The boost-arrival parameter has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi
if [ -z "$BOOST_EDGES" ] ; then
echo "WARNING: The boost-edges parameter has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi

echo ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 3 --generator 1 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/soc/1000_3/soc_1000_3_bv${BOOST_ARRIVAL}_be${BOOST_EDGES}_v.gml
./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 3 --generator 1 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/soc/1000_3/soc_1000_3_bv${BOOST_ARRIVAL}_be${BOOST_EDGES}_v.gml
echo ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 2000 --generator 2 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/rand/1000_2000/rand_1000_2000_bv${BOOST_ARRIVAL}_be${BOOST_EDGES}_v.gml
./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 2000 --generator 2 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/rand/1000_2000/rand_1000_2000_bv${BOOST_ARRIVAL}_be${BOOST_EDGES}_v.gml

exit 0
