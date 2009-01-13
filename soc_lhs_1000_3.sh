#!/bin/sh

BOOST_ARRIVAL=(2.2342 4.3963 1.6006 4.4917 2.7160 2.8760 4.0068 4.9655 1.1626 3.5982 2.5212 3.6228 4.6040 1.2711 3.3927 2.1901 3.1901 3.8062 1.5244 1.9223)
BOOST_EDGES=(4.4017 2.6024 2.8834 1.7034 1.8828 2.0664 3.4170 1.2825 3.9575 4.8915 4.7726 4.0612 1.5732 1.1870 2.3756 3.3451 3.7234 3.0044 4.2727 2.4006)

for ((i=0; i<${#BOOST_ARRIVAL[@]}; i++))
do
    echo ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 3 --generator 1 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/soc/1000_3/soc_1000_3_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
    ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 3 --generator 1 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/soc/1000_3/soc_1000_3_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
    echo ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 2000 --generator 2 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/rand/1000_2000/rand_1000_2000_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
    ./src/network/main/wevonet_main --seeds seeds.dat --num_graphs 3 --size 1000 --max_edges 2000 --generator 2 --boost_arrival ${BOOST_ARRIVAL[$i]} --boost_edge ${BOOST_EDGES[$i]} --filename ./graphs/ansoma/lhs/rand/1000_2000/rand_1000_2000_bv${BOOST_ARRIVAL[$i]}_be${BOOST_EDGES[$i]}_v.gml
done

exit 0
