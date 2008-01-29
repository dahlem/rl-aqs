reset

set xlabel "E(beta)"
set ylabel "Index"
set title "Metropolis-Hastings MCMC: beta"

set terminal postscript eps colour solid rounded
set output '2d-shdf-surrogate-beta.eps'

plot '2d-shdf-surrogate-beta.dat' w l
reset