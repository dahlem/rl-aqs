reset

set xlabel "beta"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: beta"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-beta-sampled.eps'

plot '2d-shdf-surrogate-beta-sampled.dat' w l
reset