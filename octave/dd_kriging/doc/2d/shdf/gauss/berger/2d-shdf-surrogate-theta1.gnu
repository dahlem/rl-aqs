reset

set xlabel "E(theta1)"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: theta1"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-theta1.eps'

plot '2d-shdf-surrogate-theta1.dat' w l
reset