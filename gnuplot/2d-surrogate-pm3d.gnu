#!/bin/sh
# 2d-surrogate.gnu
# create a surrogate model given the data.
#
# Usage: ./2d-surrogate <data-file> <minX> <maxX> <minY> <maxY> <Title> <xlabel> <ylabel>

USAGE="./2d-surrogate.gnu <data-file> <minX> <maxX> <minY> <maxY> <Title> <xlabel> <ylabel>";
DATAFILE=$1;
MINX=$2;
MAXX=$3;
MINY=$4;
MAXY=$5;
TITLE=$6;
XLABEL=$7;
YLABEL=$8;

# Check whether the command-line parameters are present
if [ -z "$DATAFILE" ] ; then
echo "WARNING: A data file has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi
if [ -z "$MINX" ] ; then
echo "WARNING: The lower boundary for X has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi
if [ -z "$MAXX" ] ; then
echo "WARNING: The upper boundary for X has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi
if [ -z "$MINY" ] ; then
echo "WARNING: The lower boundary for Y has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi
if [ -z "$MAXY" ] ; then
echo "WARNING: The upper boundary for Y has to be specified on the command-line."
echo "USAGE: $USAGE"
exit
fi

gnuplot << EOF

set terminal pdf enhanced colour solid rounded
set output "$DATAFILE.pdf"

set datafile separator ","

set yrange [$MINY:$MAXY]
set xrange [$MINX:$MAXX]
set autoscale z

unset key

# output of the surface
#set pm3d at s explicit
#set grid
##set key off
#unset hidden3d
#set palette rgbformulae 22,13,-31

# set pm3d depthorder explicit

# change the angle the plot is shown
set view , , 1, 1.5
set palette rgb 10,13,31
set title "$TITLE"
set xlabel "$XLABEL"
set ylabel "$YLABEL"


splot "$DATAFILE.dat" u 1:2:3:4 w pm3d, \
      "1-avgEventSys-X.dat" u 1:2:3 w p lt rgb "black"
EOF
