reset

set terminal postscript eps colour solid rounded
set output '2d-shdf-real.eps'

# Set the border lines to show (see the file gnuplot.pdf that
# came with gnuplot), and add a bit more space between the tic
# labels and "y" axis. Note that the positioning of the "xlabel"
# is in units of the character size.

#set border 1+2+4+8+16
#set zrange [-1.1:5.8]
set yrange [-1:1]
set xrange [-2:2]
#set xtics 1,1,5
#set format y "%8.1f"
#set xlabel "x"
#set ylabel "y"
#set zlabel "z" -7,-8

# Make a grid, set a coloured surface with contours, and
# select a nice colour scheme.

set dgrid3d 100,100,1
#set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
set style data lines
set contour base
set cntrparam levels 10
#set pm3d
set grid

unset key

# The next command controls the appearance of the colour
# map box, and the last makes the graph.

set view , , 1, 1.5
set palette rgb 10,13,31

set title "Six-Hump Camelback Function"
#set colorbox horiz user origin 0.1,0.8 size 0.8,.04
splot '2d-shdf-real.dat' w l ls 1
#, '2d-shdf-real.dat' w pm3d
