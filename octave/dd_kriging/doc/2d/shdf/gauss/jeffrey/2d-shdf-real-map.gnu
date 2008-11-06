set terminal pdf colour solid rounded
set output '2d-shdf-real-map.pdf'

set yrange [-1:1]
set xrange [-2:2]

#set hidden3d offset 1 trianglepattern 3 undefined 1 altdiagonal bentover
set style data lines
set contour base
set cntrparam levels 10
set pm3d explicit

unset key
set dgrid3d 100,100,1
set view map
set palette rgb 10,13,31
set title "Six-Hump Camelback Function"

splot '2d-shdf-real.dat' w pm3d
