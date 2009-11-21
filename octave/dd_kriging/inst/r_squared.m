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
## Keywords: R squared, predictive error sum of squares
## Created: 04.10.2008
## Version: 0.1
## References: PHD Thesis JD Martin, Currin et.al. "A bayesian approach
## to the design and analysis of computer experiments", and Mitchell
## et.al. "Bayesian design and analysis of computer experiments: Two
## examples."


## total sum of squares at the observed values from PHD thesis JD Martin
## equation 2.39. However, he uses some validation data set to calculate
## those. Do we need those here as well???
function ttss = sst(y)
  y_sum = sum(y.^2);
  ttss = y' * y - y_sum / rows(y);
endfunction


## Predictive error sum of squares using the cross validation errors
## from PHD thesis JD Martin equation 5.1
## X: the matrix with the design points used to fit the model
## y: the vector of responses from the experiment
## R: the correlation matrix of the input parameters
## theta: the estimated theta parameters from MLE or BA MCMC
## beta: the estimated beta parameters from MLE or BA MCMC
function press = ssp(X, y, R, beta, FUN = @(x) 1)
  e = cv_error(X, y, R, beta, FUN);
##  H = hat_matrix(X);
  press = sum(e.^2);
##press += (e(i) / (1 - H(i, i)))^2;
endfunction


function H = hat_matrix(X)
  H = X * (X' * X)^-1 * X';
endfunction


## Calculate the CV errors given the method described by Currin et.al.
## and Mitchell et.al. which is also described in Phd thesis JD Martin
## equations 3.6 and 3.7
## X: the matrix with the design points used to fit the model
## y: the vector of responses from the experiment
## R: the correlation matrix of the input parameters
## theta: the estimated theta parameters from MLE or BA MCMC
## beta: the estimated beta parameters from MLE or BA MCMC
function e = cv_error(X, y, R, beta, FUN = @(x) 1)
  R_inf = cholinv(R);
  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor
  g = R_inf * y;
  w = R_inf * F;
  r_vec = diag(R_inf);
  
  Q = diag(r_vec.^-1);
  e = Q * (g - w * beta);
endfunction


## prediction R^2 which quantifies the percent of the variability that
## the model should explain at any unobserved location from PHD Thesis
## JD Martin equation 5.6
## X: the matrix with the design points used to fit the model
## y: the vector of responses from the experiment
## theta: the estimated theta parameters from MLE or BA MCMC
## beta: the estimated beta parameters from MLE or BA MCMC
function r_p = r_pred(X, y, theta, beta, nugget=0, FUN = @(x) 1, p=2)
  R = scf_gaussianm(X, theta, nugget, p);
  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor
  y_s = [];
  
  for i = 1:rows(X)
    y_s = [y_s; krig(X(i,:), X, R, beta, theta, y, F, FUN, p)];
  endfor

  ttss = sst(y_s);
  press = ssp(X, y_s, R, beta, FUN);

  r_p = 1 - press / ttss;
endfunction


function r_p = r_predS(X, y, C, sigmaSqu, theta, beta, nugget=0, FUN = @(x) 1, p=2)
  R = scf_gaussianm(X, theta, nugget, p);
  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor
  y_s = [];
  
  for i = 1:rows(X)
    y_s = [y_s; krigS(X(i,:), X, R, C, sigmaSqu, theta, y, F, FUN, p)];
  endfor

  ttss = sst(y_s);
  press = ssp(X, y_s, (R + C), beta, FUN);

  r_p = 1 - press / ttss;
endfunction


function r_p = r_pred_nonst(X, y, xi, eta, beta, nugget=0, FUN = @(x) 1)
  R = scf_nonst_m(X, xi, eta, nugget);
  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor
  y_s = [];
  
  for i = 1:rows(X)
    y_s = [y_s; krig_nonst([X(i,1), X(i,2)], X, R, beta, xi, eta, y, f, FUN)];
  endfor

  ttss = sst(y_s);
  press = ssp(X, y_s, R, beta, FUN);

  r_p = 1 - press / ttss;
endfunction


function r_pa = r_predadj(X, y, theta, beta, nugget=0, FUN = @(x) 1)
  r_p = r_pred(X, y, theta, beta, nugget, FUN);
  r_pa = 1 - ((rows(y) - 1) / (rows(y) - columns(theta))) * (1 - r_p);
endfunction


function r_pa = r_predadjS(X, y, C, sigmaSqu, theta, beta, nugget=0, FUN = @(x) 1, p=1)
  r_p = r_predS(X, y, C, sigmaSqu, theta, beta, nugget, FUN, p);
  r_pa = 1 - ((rows(y) - 1) / (rows(y) - columns(theta))) * (1 - r_p);
endfunction
