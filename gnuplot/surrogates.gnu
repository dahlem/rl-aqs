#!/bin/sh
# 2d-surrogate.gnu
# create a surrogate model given the data.
#
# Usage: ./surrogates.gnu <data-file> <minX> <maxX> <minY> <maxY> <xlabel> <ylabel> <Title>

USAGE="./surrogates.gnu <data-file> <minX> <maxX> <minY> <maxY> <xlabel> <ylabel> <Title>";
DATAFILE=$1;
MINX=$2;
MAXX=$3;
MINY=$4;
MAXY=$5;
XLABEL=$6;
YLABEL=$7;
TITLE=$8;

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
set output "$DATAFILE.eps"

set yrange [$MINY:$MAXY]
set xrange [$MINX:$MAXX]

#set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
#set style data lines
#set contour base
#set cntrparam levels 10
#set pm3d

#unset key
set lmargin 0
set bmargin 0
set rmargin 0
set tmargin 0

set size 2.8/5., 1.7/3.

set xlabel "$XLABEL"
set ylabel "$YLABEL"

splot "rand-$DATAFILE.dat" every :4:4 title "$TITLE (ER Model)" w l,\
      "soc-$DATAFILE.dat" every :4:4 title "$TITLE (BBV Model)" w l 
EOF
