reset

set xlabel "sigma"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: sigma"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-sigma-sampled.eps'

plot '2d-shdf-surrogate-sigma-sampled.dat' w l
reset