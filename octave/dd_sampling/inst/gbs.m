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
## Keywords: Gamma Sequential Sampling
## Created: 09.02.2008
## Version: 0.1

## G = gss(k, T, mu, nu)
## gamma bridge sampling (from Avramidis, L'Ecuyer, Tremblay (2003)
## Input:
##  k  : time steps
##  T  : Sample up to time T
##  mu : the mean of the gamma distribution
##  nu : the variance of the gamma distribution
function G = gbs(k, T, mu, nu)
  G = zeros(2^k + 1, 1);
  G(2^k + 1) = 1;

  for l = 1:k
    for m = 1:2^(l - 1)
      t1 = i * T / 2^l;
      t2 = (i - 1) * T / 2^l;
      t3 = (i + 1) * T / 2^l;
      Y = beta((mu^2) * T / (nu * 2^l), (mu^2) * T / (nu * 2^l));
      G(t1) = G(t2) + (G(t3) - G(t2)) * Y;
    endfor
  endfor
endfunction
