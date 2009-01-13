#!/bin/sh
# surrogate.gnu
# create a surrogate model given the data.
#
# Usage: ./surrogate <data-file> <minX> <maxX> <minY> <maxY> <Title>

USAGE="./surrogate.gnu <data-file> <minX> <maxX> <minY> <maxY>";
DATAFILE=$1;
MINX=$2;
MAXX=$3;
MINY=$4;
MAXY=$5;
TITLE=$6;

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

set terminal postscript eps enhanced colour solid rounded
set output "$DATAFILE.eps"

set yrange [$MINY:$MAXY]
set xrange [$MINX:$MAXX]

set datafile separator ","
set palette rgb 10,13,31
set pm3d map

unset key

set title "Mean-Squared Error of a random sample of Validation Points"
#set xlabel "x_1"
#set ylabel "x_2"

splot "$DATAFILE.dat" using 1:2:4 with points ps 3 pt 7 palette
EOF
