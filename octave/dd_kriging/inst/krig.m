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
## Keywords: spatial correlation functions, gaussian, product
## correlation rule
## Created: 20.01.2008
## Version: 0.1


##function [l, beta, sigma_squared] = krig_likelihood(theta, X, y, F, nugget = 0)
##  if (nargin != 5)
##    usage("krig_likelihood(theta, X, y, F, nugget)");
##  endif

##  if (theta <= 0)
##    error("The parameter theta has to be positive");
##  endif

##  if (rows(y) != rows(X))
##    error("The vectors x and y have to have the same dimensions.");
##  endif

##  if (rows(F) != rows(X))
##    error("The vector f has to have the same dimension as matrix rows.");
##  endif

##  if (columns(theta) != columns(X))
##    error("The vector theta has to have the same dimension as matrix columns.");
##  endif

##  R = scf_gaussianm(X, theta, nugget);
##  n = rows(y);

##  beta = ((F' * (R\F))\F') * (R\y);
##  temp = y - F * beta;
##  sigma_squared = 1 / n * temp' * (R\temp);
  
##  l = - 0.5 * (n * log(2 * pi * sigma_squared) + log(det(R)) + n); ##from PhD JD Martin Eq 2.37
##endfunction

function [l, beta, sigma_squared] = krig_likelihood(theta, X, y, F, nugget = 0)
  if (nargin != 5)
    usage("krig_likelihood(theta, X, y, F, nugget)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  if (rows(y) != rows(X))
    error("The vectors x and y have to have the same dimensions.");
  endif

  if (rows(F) != rows(X))
    error("The vector f has to have the same dimension as matrix rows.");
  endif

  if (columns(theta) != columns(X))
    error("The vector theta has to have the same dimension as matrix columns.");
  endif

  R = scf_gaussianm(X, theta, nugget);
  R_inv = cholinv(R);
  n = rows(y);

  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
  temp = y - F * beta;
  sigma_squared = 1 / n * temp' * (R_inv * temp);
  
  l = - 0.5 * (n * log(2 * pi * sigma_squared) + log(det(R)) + n); ##from PhD JD Martin Eq 2.37
endfunction


function l = likelihood(y, F, beta, sigma, R, R_inv, nugget = 0)
  n = rows(y);
  temp = y - F * beta;
  l = 1 / (sqrt((2 * pi * sigma)^n * det(R))) * exp(-(temp' * R_inv * temp) / (2 * sigma));
endfunction


function y = krig(x, X, R, beta, theta, y, F, FUN = @(x) 1)
  r = scf_gaussianu(X, x, theta);
  y = FUN(x) * beta + r' * (R\(y - F * beta));
endfunction

function y = krig1(x, X, R, beta, theta, y, F, FUN = @(x) 1)
  z = x';
  r = scf_gaussianu(X, z, theta);
  y = FUN(z) * beta + r' * (R\(y - F * beta));
endfunction

function y = krig2(x, X, R, beta, theta, y, F, FUN = @(x) 1)
  z = x';
  r = scf_gaussianu(X, z, theta);
  y = -(FUN(z) * beta + r' * (R\(y - F * beta)));
endfunction


## -------------------------------------------------
## From "A nonstationary covariance based kriging method for
## metamodeling in engineering design" by Xiong et. al.
## -------------------------------------------------
function [l, beta, sigma_sq] = krig_nonst_likelihood(X, y, F, xi, eta, nugget=0)
  if (nargin != 6)
    usage("krig_nonst_likelihood(X, y, f, xi, eta, nugget)");
  endif

  if (rows(y) != rows(X))
    error("The vectors x and y have to have the same dimensions.");
  endif

  if (rows(F) != rows(X))
    error("The vector f has to have the same dimension as matrix rows.");
  endif

  R = scf_nonst_m(X, xi, eta, nugget);
  n = rows(y);
  beta = ((F' * (R\F))\F') * (R\y);
  temp = y - F * beta;
  sigma_sq = 1 / n * temp' * (R\temp);

  l = - 0.5 * (n * log(2 * pi * sigma_sq) + log(det(R)) + n); ##from PhD JD Martin Eq 2.37
endfunction


function y = krig_nonst(x, X, R, beta, xi, eta, y, F, FUN = @(x) 1)
  r = scf_nonst_u(X, x, xi, eta);
  y = FUN(x) * beta + r' * (R\(y - F * beta));
endfunction
