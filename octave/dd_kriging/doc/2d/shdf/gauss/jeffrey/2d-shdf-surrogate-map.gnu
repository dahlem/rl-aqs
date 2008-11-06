set terminal pdf colour solid rounded
set output '2d-shdf-surrogate-map.pdf'

set yrange [-1:1]
set xrange [-2:2]

#set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
set style data lines
set contour base
set cntrparam levels 10
set pm3d explicit

unset key

set view map
set palette rgb 10,13,31
set title "Six-Hump Camelback Function (Kriging Metamodel)"

splot '2d-shdf-surrogate.dat' w pm3d , 'X.dat' u 1:2:3 w p ps 0.5 pt 7 lc rgb 'black'
