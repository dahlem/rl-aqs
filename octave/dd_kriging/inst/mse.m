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
## Keywords: mean square error
## Created: 04.10.2008
## Version: 0.1
## References: PHD Thesis JD Martin


## Cross Validation : Root Mean Square Error from PHD Thesis JD Martin
## equation 5.2
## Use the CV errors
function cvmse = rmse_cv(e)
  cvmse = 0;

  for i = 1:rows(e)
    cvmse += e(i)^2;
  endfor

  cvmse /= rows(e);
  cvmse = sqrt(cvmse);
endfunction


## calculate the root mean squared error over sampled input locations
## from PHD Thesis JD Martin section 5.1.4
## X: the input locations
## y: the observed outputs
## F: the matrix of 1s
## chain: the sampled MCMC chain
## n: the numger of samples
## xmin: the minimum values for the input locations
## xmax: the maximum values for the input locations
## nugget: the nugget effect (smoothin)
function rmse = rmse_avg(X, y, F, chain, n, xmax, xmin, nugget=0)
  mse_avg = zeros(1, n);
  
  S = lhs(n, xmin, xmax);
  for i = 1:n
    mse_avg(i) = mse_average(X, S(i,:), y, f, chain, nugget);
  endfor

  rmse = sqrt(mean(mse_avg));
endfunction


## calculate the mean squared error over a specified input location
## from PHD Thesis JD Martin section 5.1.4
## X: the input locations
## x: the input location to be inspected
## y: the observed outputs
## F: the matrix of 1s
## chain: the sampled MCMC chain
## nugget: the nugget effect (smoothin)
function mse_avg = mse_average(X, x, y, F, chain, nugget=0)
  mses = zeros(1, rows(chain.theta));
  
  for i = 1:rows(chain.theta)
    mses(i) = mse_pred(X, x, y, F, chain.theta(i,:), nugget);
  endfor

  mse_avg = mean(mses);
endfunction

function mse_avg = mse_average_nonconst(X, x, y, F, chain, xi, nugget=0)
  [r,c1,c2] = size(chain.Eta);
  mses = zeros(1, c2);
  
  for i = 1:c2
    mses(i) = mse_pred_nonconst(X, x, y, F, chain.Eta(:,:,i), xi, nugget);
  endfor

  mse_avg = mean(mses);
endfunction


## calculate the estimate of the mean squared error of prediction from
## PHD Thesis JD Martin equation 2.31 and more detailed 4.9. This
## equation does not quantify the parametric uncertainty of estimating
## the theta values from the MCMC method.
## X: the input locations
## x: the input location to be inspected
## y: the observed outputs
## F: the matrix of 1s
## theta: the estimated theta value from MCMC
function mse_p = mse_pred(X, x, y, F, theta, nugget=0)
  R = scf_gaussianm(X, theta, nugget);
  R_inv = inv(R);
  r = scf_gaussianu(X, x, theta);
  sigma_sq = inv(F' * R_inv * F);
  rtemp = R_inv * r;
  z = 1 - F' * rtemp;

  mse_p = sigma_sq * (1 - r' * rtemp) + sigma_sq * (z' * sigma_sq * z);
endfunction

function mse_p = mse_pred_nonconst(X, x, y, F, eta, xi, nugget=0)
  R = scf_nonst_m(X, xi, eta, nugget);
  r = scf_nonst_u(X, x, xi, eta);
  sigma_sq = inv(F' * (R\F));
  rtemp = (R\r);
  z = 1 - F' * (rtemp);

  mse_p = sigma_sq * (1 - r' * rtemp) + sigma_sq * (z' * sigma_sq * z);
endfunction


function f = f(x)
  f = 1;
endfunction


## simulate the output distribution given the MCMC chain from PHD Thesis
## JD Martin section 4.4.4. It incorporates the structural uncertainty
## through equation 4.9 as the variance of the gaussian process and the
## parametric uncertainty through the MCMC chain.
## chain: the chain of the MCMC
## X: the input locations
## x: the input location to be inspected
## y: the observed outputs
## F: the matrix of 1s
function y_prob = y_sim(chain, X, x, y, F, nugget=0)
  y_prob = [];

  for i = 1:columns(chain.beta)
    R = scf_gaussianm(X, chain.theta(i,:), nugget);
    mean = krig(X, x, R, chain.beta(i), chain.theta(i,:), y, F, nugget)(1);
    var = mse_pred(X, x, y, F, chain.theta(i,:), nugget);
    y = normrnd(mean, var);
    y_prob = [y_prob; y];
  endfor
endfunction


## calculate the variance of the estimate (eq. 2.28) from PHD Thesis of
## JD Martin equation 2.31.
## F: the matrix of 1s
## R: the correlation matrix
## X: the input locations
## x: the input location to be inspected
## theta: the estimated theta value from MCMC
## sigma: the process variance sigma from MCMC
function mse_v = mse_var(F, R, X, x, theta, sigma, nugget=0)
  M = [0, F'; F, R]^-1;
  r = scf_gaussianu(X, x, theta, nugget);
  vec1 = [1, r'];
  vec2 = [1; r];
  mse_v = sigma * (1 - vec1 * M * vec2);
endfunction


## rescale a normal distribution to a t-distribution from the PHD of JD
## Martin equation 4.23
## y_p: simulated output values for a specified location from y_sim(...)
## var: variance of the output from mse_var(...)
## X: the input locations
## x: the input location to be inspected
## F: the matrix of 1s
## R: the correlation matrix
## y: the observed outputs
## beta: the estimated beta value from MCMC
## theta: the estimated theta value from MCMC
## lambda: the shape parameter for the t-distribution
function x = t_scale(y_p, var, X, x, F, R, y, beta, theta, lambda)
  mean = krig(X, x, R, beta, theta, y, F)(1);
  b = sqrt(var) / lambda;
  x = (y_p - mean) / b;
endfunction
