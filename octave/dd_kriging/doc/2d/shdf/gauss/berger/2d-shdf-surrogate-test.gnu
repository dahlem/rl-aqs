set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-test.eps'

# choose quality
#set samples 50, 50
#set isosamples 100

# change the angle the plot is shown
#set view 55, 45, 1, 2

# output of the surface
set pm3d# at bs
set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal #bentover
set style data #lines

# remove the box with the meaning of the color
unset colorbox

# set ranges
set zrange [-1.1:5.8]
set yrange [-1:1]
set xrange [-2:2]

set grid
set key off

# remove tics on the z-axis
unset ztics


set xlabel "xi"
set ylabel "zeta"

#set xzeroaxis linetype -1 linewidth 0.5
#set yzeroaxis linetype -1 linewidth 0.5
set xtics axis
set ytics axis

set title "Six-Hump Camelback Function (JDM Kriging)"

splot '2d-shdf-surrogate.dat' w l ls 1, '2d-shdf-surrogate.dat' w pm3d
reset