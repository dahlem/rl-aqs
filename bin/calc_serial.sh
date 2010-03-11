#!/bin/bash

s=0


for i in `find . -type d -name "graphs"`
do
  s=$(( $s + (`stat -c%Y $i/graph100.gml` - `stat -c%Y $i/graph0.gml`)))
done

echo  "Numnber of seconds in serial mode would have been: $s"

