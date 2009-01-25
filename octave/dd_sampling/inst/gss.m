## Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: Gamma Sequential Sampling
## Created: 09.02.2008
## Version: 0.1

## G = gss(k, T, mu, nu)
## gamma sequential sampling (from Avramidis, L'Ecuyer, Tremblay (2003)
## Input:
##  k  : time steps
##  T  : Sample up to time T
##  mu : the mean of the gamma distribution
##  nu : the variance of the gamma distribution
function G = gss(k, T, mu, nu)
  G = [0,0];
  h = (2^-k) * T;

  for i = 1:2^k
    Q = gamrnd(mu * i * h / nu, nu);
##    Q = gamrnd((mu^2) * h / nu, nu / mu);
    g = [i * h, G(i, 2) + Q];
    G = [G; g];
  endfor

  for i = 1:rows(G)
    G(i,2) = G(i,2) / G(2^k + 1,2);
  endfor
endfunction
