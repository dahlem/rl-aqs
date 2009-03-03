#!/bin/sh
# 2d-surrogate.gnu
# create a surrogate model given the data.
#
# Usage: ./surrogates.gnu <data-file> <minX> <maxX> <minY> <maxY> <xlabel> <ylabel> <Title>

USAGE="./surrogates.gnu <data-file> <minX> <maxX> <minY> <maxY> <xlabel> <ylabel>";
DATAFILE=$1;
MINX=$2;
MAXX=$3;
MINY=$4;
MAXY=$5;
XLABEL=$6;
YLABEL=$7;
ZLABEL=$8;

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

set terminal postscript enhanced colour solid rounded
set output "$DATAFILE.eps"

set xrange [$MINX:$MAXX]
set yrange [$MINY:$MAXY]

#set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
#set style data lines
#set contour base
#set cntrparam levels 10
#set pm3d

set lmargin 0
set bmargin 0
set rmargin 0
set tmargin 0

set size 2.8/5., 1.7/3.

set xtics 0.02
set mxtics
set ytics 0.02
set mytics
set xlabel "$XLABEL" -1,-1
set ylabel "$YLABEL" 1,0
set zlabel "$ZLABEL" 1,4

splot "rand-$DATAFILE.dat" every :4:4 title "ER Model" w l,\
      "soc-$DATAFILE.dat" every :4:4 title "BBV Model" w l 
EOF
