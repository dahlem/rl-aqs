set terminal postscript eps enhanced colour solid rounded
set output '2d-shdf-mses.eps'

set yrange [-1:1]
set xrange [-2:2]

set datafile separator ","
set palette rgb 10,13,31
set pm3d map

unset key

set title "Mean-Squared Error of a random sample of Validation Points"
set xlabel "x_1"
set ylabel "x_2"

splot '2d-shdf-surrogate-error.dat' using 1:2:4 with points ps 3 pt 7 palette
