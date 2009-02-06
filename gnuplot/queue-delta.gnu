#!/bin/sh
# 2d-surrogate.gnu
# create a surrogate model given the data.
#
# Usage: ./queue-delta <data-file><Title> <xlabel> <ylabel>

USAGE="./queue-delta.gnu <data-file> <Title> <xlabel> <ylabel>";
DATAFILE=$1;
MINX=$2;
MAXX=$3;
MINY=$4;
MAXY=$5;
XLABEL=$6;
YLABEL=$7;

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

set terminal postscript colour solid rounded
set output "$DATAFILE-delta.eps"

set yrange [$MINY:$MAXY]
set xrange [$MINX:$MAXX]

set datafile separator ","

set dgrid3d 30

set size 2.8/5., 1.7/3.

set xlabel "$XLABEL"
set ylabel "$YLABEL"

splot "soc-$DATAFILE.dat" u 1:2:5 title "Delta (ER Model)" w l,\
      "rand-$DATAFILE.dat" u 1:2:5 title "Delta (BBV Model)" w l 

EOF