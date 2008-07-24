#!/bin/sh

DEGREE="10 25 50 75 100"
OUT_DIR="./graphs/3"

for d in `echo $DEGREE`
do
	mkdir -p ${OUT_DIR}
	filename=${OUT_DIR}/100_${d}.gml
	echo "./src/network/main/wevonet_main --size 100 --max_edges $d --filename $filename"
	./src/network/main/wevonet_main --size 100 --max_edges $d --filename $filename
done

# max edges
filename=${OUT_DIR}/100_max.gml
./src/network/main/wevonet_main --size 100 $d --filename $filename

exit 0
