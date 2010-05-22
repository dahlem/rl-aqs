#!/bin/sh
# branin.gnu

gnuplot << EOF

set terminal pdf enhanced colour solid rounded
set output "branin.pdf"

set yrange [0:15]
set xrange [-5:10]
set autoscale z

unset key

# output of the surface
#set pm3d at s explicit
#set grid
##set key off
#unset hidden3d
#set palette rgbformulae 22,13,-31

#set pm3d depthorder hidden3d 2

#set pm3d depthorder hidden3d 1
#set style fill transparent solid 0.65 border
#set hidden3d

#set style fill  transparent solid 0.50 noborder lt -2
#set pm3d depthorder
#set pm3d interpolate 1,1 flush begin noftriangles hidden3d 2 corners2color mean


# set style fill  transparent solid 0.30 border
# set style line 2  linetype 2 linecolor rgb "#a0a0f0"  linewidth 0.500 pointtype 2 pointsize default pointinterval 0
# set pm3d depthorder
# set pm3d interpolate 1,1 flush begin noftriangles hidden3d 1 corners2color mean

set pm3d depthorder

# change the angle the plot is shown
set view , , 1, 1.5
set palette rgb 10,13,31
set xlabel "x"
set ylabel "y"

set isosamples 101,101

a=1
b=5.1/(4*pi**2)
c=5/pi
d=6
e=10
f=1/(8*pi)

f(x,y) = a*(y-b*x**2+c*x-d)**2+e*(1-f)*cos(x)+e

splot f(x,y) w pm3d
EOF
