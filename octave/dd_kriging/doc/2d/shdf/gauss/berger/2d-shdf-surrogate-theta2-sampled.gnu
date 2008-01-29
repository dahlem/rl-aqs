reset

set xlabel "theta2"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: theta2"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-theta2-sampled.eps'

plot '2d-shdf-surrogate-theta2-sampled.dat' w l
reset