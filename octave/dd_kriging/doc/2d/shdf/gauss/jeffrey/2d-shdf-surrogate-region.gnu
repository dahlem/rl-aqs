set terminal pdf colour solid rounded
set output '2d-shdf-surrogate-region.pdf'

set xrange [-0.5:0.5]
set yrange [0:1]

set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
set style data lines
set contour base
set cntrparam levels 10
set pm3d

unset key

# change the angle the plot is shown
set view , , 1, 1.5
set palette rgb 10,13,31
set title "Six-Hump Camelback Function (Kriging Metamodel)"

splot '2d-shdf-surrogate.dat' w l
