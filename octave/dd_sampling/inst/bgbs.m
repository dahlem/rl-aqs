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

## G = bgbs(k, T, mu, nu, theta, sigma)
## brownian gamma bridge sampling (from Avramidis, L'Ecuyer, Tremblay (2003)
## Input:
##  k  : time steps
##  T  : Sample up to time T
##  mu : the mean of the gamma distribution
##  nu : the variance of the gamma distribution
function BGBS = bgbs(k, T, mu, nu, theta, sigma)
  G = zeros(2^k + 1, 1);
  X = zeros(2^k + 1, 1);
  G(2^k + 1) = gamrnd((mu^2) * T / nu, nu / mu);
  X(2^k + 1) = normrnd(theta * G(2^k + 1), sigma^2 * G(2^k + 1));
  
  for l = 1:k
    n = 2^(k - l);
    for j = 1:2^(l - 1)
      i = (2 * j - 1) * n + 1;
      t1 = i * T / 2^k;
      t2 = (i - n) * T / 2^k;
      t3 = (i + n) * T / 2^k;
      delta1 = (t1 - t2) * mu^2 / nu;
      delta2 = (t3 - t1) * mu^2 / nu;
      Y = betarnd(delta1, delta2);
      G(i) = G(i - n) + (G(i + n) - G(i - n)) * Y;
      Z = normrnd(0, (G(i + n) - G(i)) * sigma^2 * Y);
      X(i) = Y * X(i + n) + (1 - Y) * X(i - n) + Z;
    endfor
  endfor

  BGBS.G = G;
  BGBS.X = X;
endfunction


function G = gbs(k, T, mu, nu)
  G = zeros(2^k + 1, 1);
#  G(2^k + 1) = gamrnd((mu^2) * T / nu, nu / mu);
  G(2^k + 1) = 1;
  
  for l = 1:k
    n = 2^(k - l);
    for j = 1:2^(l - 1)
      i = (2 * j - 1) * n + 1;
      t1 = i * T / 2^k;
      t2 = (i - n) * T / 2^k;
      t3 = (i + n) * T / 2^k;
      delta1 = (t1 - t2) * mu^2 / nu;
      delta2 = (t3 - t1) * mu^2 / nu;
      Y = betarnd(delta1, delta2);
      G(i) = G(i - n) + (G(i + n) - G(i - n)) * Y;
    endfor
  endfor

  for i = 1:rows(G)
    G(i) = G(i) / G(2^k + 1);
  endfor
endfunction
