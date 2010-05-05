## Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

function [l, beta, sigma_squared] = krig_likelihood(theta, X, y, F, nugget=0, p=2)
  R = scf_gaussianm(X, theta, nugget, p);
  R_inv = cholinv(R);
  n = rows(y);

  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
##  beta = ((F' * (R\F))\F') * (R\y);
  temp = y - F * beta;
##  sigma_squared = 1 / n * temp' * (R\temp);
  sigma_squared = 1 / n * temp' * (R_inv * temp);

  l = - 0.5 * (n * log(2 * pi * sigma_squared) + log(det(R)) + n); ##from PhD JD Martin Eq 2.37
endfunction

function y = krig_likelihood_min(theta, X, y, F, nugget=0, p=2)
  theta = theta';
  R = scf_gaussianm(X, theta, nugget, p);
  R_inv = cholinv(R);
  n = rows(y);

  beta = ((F' * R_inv * F)\F') * R_inv * y;
  temp = y - F * beta;
  sigma_squared = 1 / n * temp' * R_inv * temp;

##  y = 0.5 * (log(sigma_squared * det(R)) + temp' * R_inv * temp); ##from PhD JD Martin Eq 2.37
  y = 0.5 * (n * log(2 * pi * sigma_squared) + log(det(R)) + n); ##from PhD JD Martin Eq 2.37
endfunction

function y = krig_likelihood_min2(thetas, X, y, F, nugget=0, p=2)
  theta = thetas(2:end)';
  sigmaSquared = thetas(1);

  R = sigmaSquared * scf_gaussianm(X, theta, nugget, p);
  R_inv = cholinv(R);
  n = rows(y);

  beta = ((F' * R_inv * F)\F') * R_inv * y;
  temp = y - F * beta;

  y = 0.5 * (log(det(R)) + temp' * R_inv * temp); ##from PhD JD Martin Eq 2.37
  ##  y = 0.5 * (n * log(2 * pi * sigma_squared) + log(det(R)) + n); ##from PhD JD Martin Eq 2.37
endfunction


## from better simulation metamodelling, the why, what, and how of
## stochastic kriging by Staum
function [l, beta] = krig_likelihoodStochastic(theta, C, X, y, F, sigmaSquared, nugget = 0, p=2)
  if (nargin != 8)
    usage("krig_likelihood(theta, C, X, y, F, nugget)");
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

  ## compute the inverse of the covariance matrices
  R = sigmaSquared * scf_gaussianm(X, theta, nugget, p) + C;
  R_inv = cholinv(R);
  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
  temp = y - F * beta;
  n = rows(y);

  l = - 0.5 * (n * log(2 * pi) + log(det(R)) + temp' * R_inv * temp); ##Staum ##eq. (14)
endfunction

function [l, beta] = krig_likelihoodStochasticS(theta, C, X, y, F, sigmaSquared, nugget = 0, p=2)
  S = sigmaSquared * scf_gaussianm(X, theta, nugget, p) + C;
  U = chol(S);

  ## invert it via Cholesky factorization
  L = U';
  Linv = inv(L);
  Sinv = Linv'*Linv;

  ## the optimal beta given theta and tau2
  beta = inv(F'*Sinv*F)*(F'*(Sinv*y)); 
  Z = L\(y-F*beta);

  ## negative log likelihood
  n = rows(y);
  f = (log(det(L)) + 0.5*Z'*Z + 0.5*n*log(2*pi));
endfunction


## from better simulation metamodelling, the why, what, and how of
## stochastic kriging by Ankeman eq. (20)
function y = krigLikeliStMin(thetas, C, X, y, F, nugget=0, p=2)
  theta = thetas(2:end)';
  sigmaSquared = thetas(1);

  ## compute the inverse of the covariance matrices
  R = sigmaSquared * scf_gaussianm(X, theta, nugget, p) + C;
  R_inv = cholinv(R);
  left = F' * R_inv * F;
  left_inv = cholinv(left);
  
  b = left_inv * F' * R_inv * y;
  temp = y - F * b;
  n = rows(y);

  y = -(-0.5 * log((2 * pi)^(n/2)) - 0.5 * log(det(R)) - 0.5 * temp' * R_inv * temp); ##Ankeman eq. (20)
endfunction

function y = krigLikeliGaussStMin(thetas, C, X, y, F, nugget=0, p=2)
  theta = thetas(2:end)';
  sigmaSquared = thetas(1);

  ## compute the inverse of the covariance matrices
  R = sigmaSquared * scf_gaussianm(X, theta, nugget, p) + C;
  R_inv = cholinv(R);
  left = F' * R_inv * F;
  left_inv = cholinv(left);
  
  b = left_inv * F' * R_inv * y;
  temp = y - F * b;
  n = rows(y);

  y = 1/(sqrt((2*pi*sigmaSquared)^n * det(R))) * e^(-temp'*R_inv*temp/2*sigmaSquared);
endfunction


function l = logLikeli(beta, theta, aTheta, bTheta, sigmaSq, aSigma, bSigma, X, y, F, nugget = 0)
  ## params are in log domain, so do back transformation
  sigmaSq.cand = log(sigmaSq);
  beta.cand = beta;
  theta.cand = logitinv(theta, aTheta, bTheta);

  R = sigmaSq.cand * scf_gaussianm(X, theta.cand, nugget);
  R_inv = cholinv(R);
  n = rows(y);
  temp = y - F * beta.cand;
  l = (-(aSigma + 1) * log(sigmaSq.cand) - bSigma/sigmaSq.cand + log(sigmaSq.cand) + sum(log(theta.cand - aTheta) + log(bTheta - theta.cand)) -0.5 * log(det(R)) -0.5 * (temp' * R_inv * temp));
endfunction


function l = likelihood(y, F, beta, sigma, R, R_inv, nugget = 0)
  n = rows(y);
  temp = y - F * beta;
  l = 1 / (sqrt((2 * pi * sigma)^n * det(R))) * exp(-(temp' * R_inv * temp) / (2 * sigma));
endfunction


function y = krig(x, X, R, beta, theta, y, F, FUN = @(x) 1, p=2)
  r = scf_gaussianu(X, x, theta, p);
  y = FUN(x) * beta + r' * (R\(y - F * beta));
endfunction

## from Ankeman
function f = krigS(x, X, R, C, sigmaSqu, theta, y, F, FUN = @(x) 1, p=2)
  R = sigmaSqu * R + C;
  R_inv = cholinv(R);
  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
  r = scf_gaussianu(X, x, theta, p) * sigmaSqu;

  f = FUN(x) * beta + r' * R_inv * (y - F * beta);
endfunction

##function y = krigS(x, X, R, C, sigmaSqu, theta, y, F, FUN = @(x) 1, p=2)
##  R_inv = cholinv(sigmaSqu * R + C);
##  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
##  r = scf_gaussianu(X, x, theta, p)/sigmaSqu;
##  y = FUN(x) * beta + r' * ((R + C/sigmaSqu)\(y - F * beta));
##endfunction


## from Ankeman
function y = krigS1(x, X, R, C, sigmaSqu, theta, y, F, FUN = @(x) 1, p=2)
  x = x';
  R = sigmaSqu * R + C;
  R_inv = cholinv(R);
  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
  r = scf_gaussianu(X, x, theta, p) * sigmaSqu;

  y = FUN(x) * beta + r' * R_inv * (y - F * beta);
endfunction

function y = krigS2(x, X, R, C, sigmaSqu, theta, y, F, FUN = @(x) 1, p=2)
  x = x';
  R = sigmaSqu * R + C;
  R_inv = cholinv(R);
  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
  r = scf_gaussianu(X, x, theta, p) * sigmaSqu;

  y = -(FUN(x) * beta + r' * R_inv * (y - F * beta));
endfunction


##function y = krigS1(x, X, R, C, sigmaSqu, theta, y, F, FUN = @(x) 1, p=2)
##  z = x';
##  R_inv = cholinv(R + C);
##  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
##  r = scf_gaussianu(X, z, theta, p)/sigmaSqu;
##  y = FUN(z) * beta + r' * ((R/sigmaSqu + C/sigmaSqu)\(y - F * beta));
##endfunction


function y = krigSMinBoost(x, boost, X, R, C, sigmaSqu, theta, y, F, FUN = @(x) 1, p=2)
  z = x';
  z = [boost; z];
  R = sigmaSqu * R + C;
  R_inv = cholinv(R);
  beta = ((F' * (R_inv * F))\F') * (R_inv * y);
  r = scf_gaussianu(X, z, theta, p) * sigmaSqu;

  y = FUN(z) * beta + r' * R_inv * (y - F * beta);
endfunction


function y = krig1(x, X, R, beta, theta, y, F, FUN = @(x) 1, p=2)
  z = x';
  r = scf_gaussianu(X, z, theta, p);
  y = FUN(z) * beta + r' * (R\(y - F * beta));
endfunction


function y = krig2(x, X, R, beta, theta, y, F, FUN = @(x) 1, p=2)
  z = x';
  r = scf_gaussianu(X, z, theta, p);
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
