## Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
##  
## This file is free software; as a special exception the author gives
## unlimited permission to copy and/or distribute it, with or without 
## modifications, as long as this notice is preserved.
## 
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
## implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## Author: Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
## Maintainer: Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
## Keywords: spatial correlation functions, gaussian, product
## correlation rule
## Created: 20.01.2008
## Version: 0.1


## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0.beta is a vector of
## the initial values of beta, x0.theta is a vector of initial values
## for theta, x0.sigma is the initial value for sigma, X is the matrix
## of n observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function  mcmc_mh(s1, x0beta, x0theta, x0sigma, X, y, n)
endfunction
