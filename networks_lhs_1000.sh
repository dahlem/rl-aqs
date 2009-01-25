#!/bin/sh

BOOST_ARRIVAL=(1.5000 1.0191 1.0500 1.0341)
BOOST_EDGES=(1.2852 1.3801 1.0312 1.0293)

for ((i=0; i<${#BOOST_ARRIVAL[@]}; i++))
do
    echo ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 3 --generator 1 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/soc/1000_3/soc_1000_3_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
    ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 3 --generator 1 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/soc/1000_3/soc_1000_3_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
    echo ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 2000 --generator 2 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/rand/1000_2000/rand_1000_2000_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
    ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 2000 --generator 2 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/rand/1000_2000/rand_1000_2000_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
done

exit 0
