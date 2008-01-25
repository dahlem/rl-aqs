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


function [l, beta, sigma_squared] = krig_likelihood(theta, X, y, f)
  if (nargin != 4)
    usage("krig_likelihood(theta, X, y, f)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  if (rows(y) != rows(X))
    error("The vectors x and y have to have the same dimensions.");
  endif

  if (rows(f) != rows(X))
    error("The vector f has to have the same dimension as matrix rows.");
  endif

  if (columns(theta) != columns(X))
    error("The vector theta has to have the same dimension as matrix columns.");
  endif

  R = scf_gaussianm(X, theta);
  R_inf = R^-1;
  n = rows(y);
  beta = (f' * R_inf * f)^-1 * f' * R_inf * y;
  temp = y - f * beta;
  sigma_squared = 1 / n * temp' * R_inf * temp;

  l = - (n/2) * log(2 * pi * sigma_squared) - 1/2 * log(det(R)) - 1/(2 * sigma_squared) * (temp' * R_inf * temp);
  
endfunction
