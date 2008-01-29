reset

set xlabel "theta1"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: theta1"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-theta1-sampled.eps'

plot '2d-shdf-surrogate-theta1-sampled.dat' w l
reset