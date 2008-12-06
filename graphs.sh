#!/bin/sh

DEGREE="2 3 4 5 6"
NODES="25 50 100 250 500 1000 2000 3000 4000 5000 10000"
OUT_DIR="./graphs/5"

for n in `echo $NODES`
do
    for d in `echo $DEGREE`
    do
	mkdir -p ${OUT_DIR}
	filename=${OUT_DIR}/${n}_${d}.gml
	echo "./src/network/main/wevonet_main --weight 1 --seeds seeds.dat --size $n --max_edges $d --filename $filename"
	./src/network/main/wevonet_main --weight 1 --seeds seeds.dat --size $n --max_edges $d --filename $filename
    done
done

exit 0
