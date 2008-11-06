reset

set xlabel "E(sigma)"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: sigma"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-sigma.eps'

plot '2d-shdf-surrogate-sigma.dat' w l
