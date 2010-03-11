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

set terminal postscript eps enhanced colour solid rounded size 2.8,2.8 10
set output "$DATAFILE.eps"

set datafile separator ","

set yrange [$MINY:$MAXY]
set xrange [$MINX:$MAXX]
set autoscale z

# output of the surface
set pm3d at s explicit
set grid
set key off
unset hidden3d
set palette rgbformulae 22,13,-31

#set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
#set style data lines
#set contour base
#set cntrparam levels 10
#set pm3d
# set grid nopolar
# set grid xtics ytics ztics
#unset key

# change the angle the plot is shown
set view , , 1, 1.5
#set palette rgb 10,13,31
set title "$TITLE"
set xlabel "$XLABEL"
set ylabel "$YLABEL"

splot "$DATAFILE.dat" u 1:2:3:4 w pm3d
EOF
