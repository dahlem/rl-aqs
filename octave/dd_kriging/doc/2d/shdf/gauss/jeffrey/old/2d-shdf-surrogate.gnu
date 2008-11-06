set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate.eps'

set yrange [-1:1]
set xrange [-2:2]

set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
set style data lines
set contour base
set cntrparam levels 10
set pm3d
set grid

unset key

# remove the box with the meaning of the color
#unset colorbox

# change the angle the plot is shown
set view , , 1, 1.5
set palette rgb 10,13,31

set title "Six-Hump Camelback Function (Kriging Metamodel)"
#set colorbox horiz user origin 0.1,0.8 size 0.8,.04
splot '2d-shdf-surrogate.dat' w l
reset
