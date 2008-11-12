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
## Keywords: Kriging, 2D Six-Hump Camelback Function
## Created: 22.01.2008
## Version: 0.1

global X = [
	    -2.0, 0.0;
	    -1.8, -0.7;
	    -1.6, 0.5;
	    -1.4, 1.0;
	    -1.2, -0.3;
	    -1.0, 0.2;
	    -0.8, -0.8;
	    -0.6, 0.7;
	    -0.4, -0.4;
	    -0.2, 0.1;
	    0.0, -0.9;
	    0.2, 0.9;
	    0.4, 0.4;
	    0.6, -0.5;
	    0.8, -0.1;
	    1.0, -1.0;
	    1.2, 0.8;
	    1.4, 0.3;
	    1.6, -0.6;
	    1.8, -0.2;
	    2.0, 0.6
	    ];

function x_bar_new = welford_mean(n, x_bar, x)
  d = x - x_bar;
  x_bar_new = x_bar + d / n;
endfunction


function mean_chain = calc_mean(chain)
  mean_chain.beta = zeros(1, chain.accepted);
  mean_chain.theta = zeros(chain.accepted, columns(chain.theta));
  mean_chain.sigma = zeros(1, chain.accepted);;
  mean_chain.l = zeros(1, chain.accepted);;

  # first element calculation
  mean_chain.beta(1) = welford_mean(1, 0, chain.beta(1));
  mean_chain.l(1) = welford_mean(1, 0, chain.l(1));
  mean_chain.sigma(1) = welford_mean(1, 0, chain.sigma(1));
  for j = 1:columns(chain.theta)
    mean_chain.theta(1,j) = welford_mean(1, 0, chain.theta(1,j));
  endfor

  # calc to the end of the chain
  for i = 2:chain.accepted
    mean_chain.beta(i) = welford_mean(i, mean_chain.beta(i - 1), chain.beta(i));
    mean_chain.sigma(i) = welford_mean(i, mean_chain.sigma(i - 1), chain.sigma(i));
    mean_chain.l(i) = welford_mean(i, mean_chain.l(i - 1), chain.l(i));

    for j = 1:columns(chain.theta)
      mean_chain.theta(i,j) = welford_mean(i, mean_chain.theta(i-1, j), chain.theta(i,j));
    endfor
  endfor
endfunction


function mean_chain = calc_mean_nonst(chain)
  mean_chain.beta = zeros(1, chain.accepted);
  mean_chain.eta = zeros(rows(chain.eta), columns(chain.eta), chain.accepted);
  mean_chain.sigma = zeros(1, chain.accepted);

				# first element calculation
  mean_chain.beta(1) = welford_mean(1, 0, chain.beta(1));
  mean_chain.sigma(1) = welford_mean(1, 0, chain.sigma(1));
  for i = 1:rows(chain.eta)
    for j = 1:columns(chain.eta)
      mean_chain.eta(i, j, 1) = welford_mean(1, 0, chain.eta(i,j,1));
    endfor
  endfor
				# calc to the end of the chain
  for i = 2:chain.accepted
    mean_chain.beta(i) = welford_mean(i, mean_chain.beta(i - 1), chain.beta(i));
    mean_chain.sigma(i) = welford_mean(i, mean_chain.sigma(i - 1), chain.sigma(i));
  endfor
  
  for l = 1:rows(chain.eta)
    for k = 1:columns(chain.eta)
      for i = 2:chain.accepted
	mean_chain.eta(l,k,i) = welford_mean(i, mean_chain.eta(l, k, i-1), chain.eta(l,k,i));
      endfor
    endfor
  endfor
endfunction


function y = shdf(x1, x2)
  if ((x1 < -2) || (x1 > 2))
    error("x1 of the shcf function is defined over [-2;2]");
  endif
  if ((x2 < -1) || (x2 > 1))
    error("x2 of the shcf function is defined over [-1;1]");
  endif
  
  y = (4 - 2.1 * x1^2 + (x1^4)/3) * x1^2 + x1 * x2 + (-4 + 4 * x2^2) * x2^2;
endfunction


function shdf_jaydmartin()
  global X;
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  outfile = "../doc/2d/shdf/gauss/jeffrey/2d-shdf-jdm.dat";

  points = [];
  
  for x1 = x1_vec
    for x2 = x2_vec
      points = [points, x1, x2, shdf(x1, x2)];
    endfor
  endfor

  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f %14.6f %14.6f\n", points);
  fclose(fd);
  
endfunction


function shdf_jaydmartin_krig()
  global X;

  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  y = [];
  
  for i = 1:columns(x1_vec)
    y = [y; shdf(x1_vec(i), x2_vec(i))];
  endfor

  samples = 100;

  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  outfile = "../doc/2d/shdf/gauss/jeffrey/2d-shdf-jdm-krig.dat";

  points = [];
  beta = 1.40655;
  theta = [0.102542, 2.03416];
  R = scf_gaussianm(X, theta);
  f = ones(rows(X), 1);
  
  points = [];
  
  for x1 = x1_vec
    for x2 = x2_vec
      points = [points, x1, x2, krig([x1, x2], X, R, beta, theta, y, f)(1)];
    endfor
  endfor

  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f %14.6f %14.6f\n", points);
  fclose(fd);
  
endfunction


function points = shdf_real()
  outfile = "../doc/2d/shdf/gauss/jeffrey/1/2d-shdf-real.dat";
  samples = 100;

  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  points = [];
  
  for x1 = x1_vec
    for x2 = x2_vec
      points = [points, x1, x2, shdf(x1, x2)];
    endfor
  endfor
  
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f %14.6f %14.6f\n", points);
  fclose(fd);
    
endfunction


function shdf_coordinates()
  outfile = "../doc/2d/shdf/gauss/jeffrey/2d-shdf-coordinates.dat";
  global X;
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  y = [];
  
  for i = 1:columns(x1_vec)
    y = [y; shdf(x1_vec(i), x2_vec(i))];
  endfor

  fd = fopen(outfile, "wt");
  for i = 1:columns(x1_vec)
    fprintf(fd, "%14.6f %14.6f %14.6f\n", x1_vec(i), x2_vec(i), y(i));
  endfor
  fclose(fd);
endfunction


function chain = shdf_krig(dir = "../doc/2d/shdf/gauss/jeffrey/4", burn_rate = 10000, tau = 19, prior = "Jeffrey", nugget = 0)
  global X;
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  y = [];
  
  for i = 1:columns(x1_vec)
    y = [y; shdf(x1_vec(i), x2_vec(i))];
  endfor
  
  outfile = [dir "/X.dat"];
  fd = fopen(outfile, "wt");
  fprintf(fd, "x1 x2 y\n");
  for i = 1:rows(X)
    fprintf(fd, "%.6f %.6f %.6f\n", X(i,1), X(i,2), y(i));
  endfor
  fclose(fd);
  
  chain = mcmc_mh(1.3, [1, 1], X, y, 70000, prior, nugget);

  serialiseOrigChain(dir=dir, prefix="2d-shdf", chain);

  # inspect the equilibration
  burn.beta = chain.beta(burn_rate:end);
  burn.sigma = chain.sigma(burn_rate:end);
  burn.theta = zeros(chain.accepted - burn_rate + 1, 2);
  burn.theta(:,1) = chain.theta(burn_rate:end,1);
  burn.theta(:,2) = chain.theta(burn_rate:end,2);

  # inspect the autocorrelation time and the respective errors
  outfile = [dir "/2d-shdf-autocorr.dat"];
  fd = fopen(outfile, "wt");
  fprintf(fd, "var,F_est,F_error,F_error_error,tauint,tauint_error,wopt\n");
  [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.beta',1.5,columns(burn.beta),"beta",1);
  fprintf(fd, "beta,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", value,dvalue,ddvalue,tauint,dtauint,wopt);
  [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.sigma',1.5,columns(burn.sigma),"sigma",1);
  fprintf(fd, "sigma,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", value,dvalue,ddvalue,tauint,dtauint,wopt);
  [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.theta(:,1),1.5,rows(burn.theta),"theta1",1);
  fprintf(fd, "theta1,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", value,dvalue,ddvalue,tauint,dtauint,wopt);
  [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.theta(:,2),1.5,rows(burn.theta),"theta2",1);
  fprintf(fd, "theta2,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", value,dvalue,ddvalue,tauint,dtauint,wopt);
  fclose(fd);
  
  sampled.beta = burn.beta(1:tau:end);
  sampled.sigma = burn.sigma(1:tau:end);
  sampled.theta = zeros(rows(sampled.sigma), 2);
  sampled.theta = burn.theta(1:tau:end,:);

  serialiseSampledChain(dir, prefix="2d-shdf", sampled);
  
  samples = 100;
  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  f = ones(rows(X), 1);
  theta = mean(sampled.theta);
  beta = mean(sampled.beta);
  R = scf_gaussianm(X, theta, nugget);

  outfile = [dir "/2d-shdf-surrogate.dat"];
  fd = fopen(outfile, "wt");
  for x1 = x1_vec
    for x2 = x2_vec
      y_krig = krig([x1, x2], X, R, beta, theta, y, f)(1);
      fprintf(fd, "%14.6f %14.6f %14.6f\n", x1, x2, y_krig);
    endfor
    fprintf(fd, "\n");
  endfor
  fclose(fd);

  likelihoods_actual(X, y, f, sampled, 0, dir, prefix="2d-shdf");
  

  ## start the next iteration... that means increment the dir field
  outfile = [dir "/2d-shdf-surrogate-error.dat"];
  S = lhs(50, [-2,-1],[2,1]);
  fd = fopen(outfile, "wt");
  fprintf(fd, "x1,x2,y_krig,mse\n");
  for i = 1:50
    mse_avg = mse_average(X, S(i,:), y, f, sampled, 0);
    y_krig = krig(S(i,:), X, R, beta, theta, y, f)(1);
    fprintf(fd, "%.6f,%.6f,%.6f,%.10f\n", S(i,1), S(i,2), y_krig, mse_avg);
  endfor
  fclose(fd);
  
  # choose the point with the highest mse to improve the metamodel
  infile = [dir "/2d-shdf-surrogate-error.dat"];
  m = csvread(infile);
  m = m(2:end,:); # remove the empty line caused by the header
  rmse = sqrt(mean(m(:,4)));
  r_p = r_pred(X, y, theta, beta, nugget=0);
  max_mse = max(m(:,4));
  m(find (m(:,4) == max_mse),1:2)
  X  = [X; m(find (m(:,4) == max_mse),1:2)]
  
endfunction


function chain = shdf_nonst_krig(dir = "../doc/2d/shdf/gauss/jeffrey/4", burn_rate = 10000, tau = 19, prior = "Jeffrey")
  global X;

  var = 1.3;
  xmin = [-2, -1];
  xmax = [2, 1];
  
  k = 1;
  xi = zeros(k+1, columns(X));
  eta = ones(k+1, columns(X));

  for l = 1:columns(X)
    delta = abs(xmax(l) - xmin(l)) / k;
    xi(:,l) = xmin(l):delta:xmax(l);
  endfor

  for r = 1:rows(eta)
    for c = 1:columns(eta)
      do
        eta(r,c) = normrnd(0, var);
      until (eta(r,c) > 0)
    endfor
  endfor
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  y = [];
  
  for i = 1:columns(x1_vec)
    y = [y; shdf(x1_vec(i), x2_vec(i))];
  endfor
  
  ## reset dir
  outfile = [dir "/X.dat"];
  fd = fopen(outfile, "wt");
  fprintf(fd, "x1 x2 y\n");
  for i = 1:rows(X)
    fprintf(fd, "%.6f %.6f %.6f\n", X(i,1), X(i,2), y(i));
  endfor
  fclose(fd);

  chain = mcmc_nonst_mh(var, eta, 1, X, y, 10000, prior, xi);

  serialiseOrigChainNonst(dir=dir, prefix="2d-shdf", chain);

  # inspect the equilibration
  burn.beta = chain.beta(burn_rate:end);
  burn.sigma = chain.sigma(burn_rate:end);
  burn.eta = zeros(rows(chain.eta), columns(chain.eta), chain.accepted - burn_rate + 1);

  for i = 1:rows(chain.eta)
    for j = 1:columns(chain.eta)
      burn.eta(i, j) = chain.eta(i, j, burn_rate:end);
    endfor
  endfor

  # inspect the autocorrelation time and the respective errors
  outfile = [dir "/2d-shdf-autocorr.dat"];
  fd = fopen(outfile, "wt");
  fprintf(fd, "var,F_est,F_error,F_error_error,tauint,tauint_error,wopt\n");
  [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.beta',1.5,columns(burn.beta),"beta",1);
  fprintf(fd, "beta,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", value,dvalue,ddvalue,tauint,dtauint,wopt);
  [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.sigma',1.5,columns(burn.sigma),"sigma",1);
  fprintf(fd, "sigma,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", value,dvalue,ddvalue,tauint,dtauint,wopt);

  for i = 1:rows(sampled.eta)
    for j = 1:columns(sampled.eta)
      var = ["eta" i "_" j]
      [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(burn.eta(i,j,:),1.5,rows(burn.sigma),var,1);
      fprintf(fd, "eta%d_%d,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", i,j,value,dvalue,ddvalue,tauint,dtauint,wopt);
    endfor
  endfor
  fclose(fd);
  
  sampled.beta = burn.beta(1:tau:end);
  sampled.sigma = burn.sigma(1:tau:end);
  sampled.eta = zeros(rows(chain.eta), columns(chain.eta), rows(sampled.sigma));
  sampled.eta = burn.eta(:,:,1:tau:end);

  serialiseSampledChain(dir, prefix="2d-shdf", sampled);
  
  samples = 100;
  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  f = ones(rows(X), 1);
  theta = mean(sampled.theta);
  beta = mean(sampled.beta);
  R = scf_gaussianm(X, theta, nugget);

  outfile = [dir "/2d-shdf-surrogate.dat"];
  fd = fopen(outfile, "wt");
  for x1 = x1_vec
    for x2 = x2_vec
      y_krig = krig([x1, x2], X, R, beta, theta, y, f)(1);
      fprintf(fd, "%14.6f %14.6f %14.6f\n", x1, x2, y_krig);
    endfor
    fprintf(fd, "\n");
  endfor
  fclose(fd);

  likelihoods_actual(X, y, f, sampled, 0, dir, prefix="2d-shdf");
  

  ## start the next iteration... that means increment the dir field
  outfile = [dir "/2d-shdf-surrogate-error.dat"];
  S = lhs(50, [-2,-1],[2,1]);
  fd = fopen(outfile, "wt");
  fprintf(fd, "x1,x2,y_krig,mse\n");
  for i = 1:50
    mse_avg = mse_average(X, S(i,:), y, f, sampled, 0);
    y_krig = krig(S(i,:), X, R, beta, theta, y, f)(1);
    fprintf(fd, "%.6f,%.6f,%.6f,%.10f\n", S(i,1), S(i,2), y_krig, mse_avg);
  endfor
  fclose(fd);
  
  # choose the point with the highest mse to improve the metamodel
  infile = [dir "/2d-shdf-surrogate-error.dat"];
  m = csvread(infile);
  m = m(2:end,:); # remove the empty line caused by the header
  rmse = sqrt(mean(m(:,4)));
  r_p = r_pred(X, y, theta, beta, nugget=0);
  max_mse = max(m(:,4));
  m(find (m(:,4) == max_mse),1:2)
  X  = [X; m(find (m(:,4) == max_mse),1:2)]
  
endfunction


function test_surrogate(X, R, beta, theta, y, f)
  for i = 1:rows(X)
    resK = krig([X(i,1), X(i,2)], X, R, beta, theta, y, f);
    resR = shdf(X(i,1), X(i,2));

    if (abs(resK(1) - resR) > 0.00001)
      warning("The surrogate model did not return a correct result");
    endif
  endfor
endfunction


function shdf_real_test()
  outfile = "../doc/2d/shdf/gauss/jeffrey/2d-shdf-real-test.dat";
  samples = 100;

  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  fd = fopen(outfile, "wt");
  for x1 = x1_vec
    for x2 = x2_vec
      fprintf(fd, "%14.6f %14.6f %14.6f\n", x1, x2, shdf(x1, x2));
    endfor
    fprintf(fd, "\n");
  endfor

  fclose(fd);
    
endfunction


function serialiseOrigChain(dir, prefix="2d-shdf", chain)
  fd = fopen([dir, "/", prefix, "-chain-beta.dat"], "wt");
  fprintf(fd, "beta\n");
  fprintf(fd, "%.10f\n", chain.beta);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-sigma.dat"], "wt");
  fprintf(fd, "sigma\n");
  fprintf(fd, "%.10f\n", chain.sigma);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-theta1.dat"], "wt");
  fprintf(fd, "theta1\n");
  fprintf(fd, "%.10f\n", chain.theta(:,1));
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-theta2.dat"], "wt");
  fprintf(fd, "theta2\n");
  fprintf(fd, "%.10f\n", chain.theta(:,2));
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-posterior.dat"], "wt");
  fprintf(fd, "posterior\n");
  fprintf(fd, "%.10f\n", chain.l);
  fclose(fd);

  mean_chain = calc_mean(chain);
  fd = fopen([dir, "/", prefix, "-chain-beta-mean.dat"], "wt");
  fprintf(fd, "mean_beta\n");
  fprintf(fd, "%.10f\n", mean_chain.beta);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-sigma-mean.dat"], "wt");
  fprintf(fd, "mean_sigma\n");
  fprintf(fd, "%.10f\n", mean_chain.sigma);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-theta1-mean.dat"], "wt");
  fprintf(fd, "mean_theta1\n");
  fprintf(fd, "%.10f\n", mean_chain.theta(:,1));
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-theta2-mean.dat"], "wt");
  fprintf(fd, "mean_theta2\n");
  fprintf(fd, "%.10f\n", mean_chain.theta(:,2));
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-posterior-mean.dat"], "wt");
  fprintf(fd, "mean_posterior\n");
  fprintf(fd, "%.10f\n", mean_chain.l);
  fclose(fd);
endfunction


function serialiseOrigChainNonst(dir, prefix="2d-shdf", chain)
  fd = fopen([dir, "/", prefix, "-chain-nonst-beta.dat"], "wt");
  fprintf(fd, "beta\n");
  fprintf(fd, "%.10f\n", chain.beta);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-nonst-sigma.dat"], "wt");
  fprintf(fd, "sigma\n");
  fprintf(fd, "%.10f\n", chain.sigma);
  fclose(fd);

  for i = 1:rows(chain.eta)
    for j = 1:columns(chain.eta)
      var = sprintf("eta%d_%d", i, j);
      fd = fopen([dir, "/", prefix, "-nonst-chain-", var, ".dat"], "wt");
      fprintf(fd, "%s\n", var);
      fprintf(fd, "%.10f\n", chain.eta(i,j,:));
      fclose(fd);
    endfor
  endfor

  mean_chain = calc_mean_nonst(chain);
  fd = fopen([dir, "/", prefix, "-chain-nonst-beta-mean.dat"], "wt");
  fprintf(fd, "mean_beta\n");
  fprintf(fd, "%.10f\n", mean_chain.beta);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-nonst-sigma-mean.dat"], "wt");
  fprintf(fd, "mean_sigma\n");
  fprintf(fd, "%.10f\n", mean_chain.sigma);
  fclose(fd);

  for i = 1:rows(chain.eta)
    for j = 1:columns(chain.eta)
      var = sprintf("eta%d_%d", i, j);
      fd = fopen([dir, "/", prefix, "-nonst-chain-", var, "-mean.dat"], "wt");
      fprintf(fd, "%s\n", var);
      fprintf(fd, "%.10f\n", chain.eta(i,j,:));
      fclose(fd);
    endfor
  endfor

endfunction


function serialiseSampledChain(dir, prefix="2d-shdf", chain)
  fd = fopen([dir, "/", prefix, "-chain-beta-sampled.dat"], "wt");
  fprintf(fd, "sampled_beta\n");
  fprintf(fd, "%.10f\n", chain.beta);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-sigma-sampled.dat"], "wt");
  fprintf(fd, "sampled_sigma\n");
  fprintf(fd, "%.10f\n", chain.sigma);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-theta1-sampled.dat"], "wt");
  fprintf(fd, "sampled_theta1\n");
  fprintf(fd, "%.10f\n", chain.theta(:,1));
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-theta2-sampled.dat"], "wt");
  fprintf(fd, "sampled_theta2\n");
  fprintf(fd, "%.10f\n", chain.theta(:,2));
  fclose(fd);
endfunction


function serialiseSampledChainNonst(dir, prefix="2d-shdf", chain)
  fd = fopen([dir, "/", prefix, "-chain-beta-sampled.dat"], "wt");
  fprintf(fd, "sampled_beta\n");
  fprintf(fd, "%.10f\n", chain.beta);
  fclose(fd);
  fd = fopen([dir, "/", prefix, "-chain-sigma-sampled.dat"], "wt");
  fprintf(fd, "sampled_sigma\n");
  fprintf(fd, "%.10f\n", chain.sigma);
  fclose(fd);
  
  for i = 1:rows(chain.eta)
    for j = 1:columns(chain.eta)
      var = ["sampled_eta" i "_" j]
      fd = fopen([dir, "/", prefix, "-chain-", var, ".dat"], "wt");
      fprintf(fd, "%s\n", var);
      fprintf(fd, "%.10f\n", chain.eta(i,j,:));
      fclose(fd);
    endfor
  endfor

endfunction


function likelihoods_actual(X, y, f, chain, nugget=0, dir, prefix="2d-shdf")
  n = columns(chain.beta);
  likeli.beta = zeros(1, n);
  likeli.theta = zeros(n, columns(chain.theta));
  likeli.sigma = zeros(1, n);

  est.beta = mean(chain.beta);
  est.sigma = mean(chain.sigma);
  est.theta = mean(chain.theta);

  # calc the beta likelihoods
  R = scf_gaussianm(X, est.theta, nugget);
  R_inv = R^-1;
  fd = fopen([dir, "/", prefix, "-chain-beta-likelihood.dat"], "wt");
  fprintf(fd, "likelihood_beta,beta\n");
  for i = 1:n
    likeli.beta(i) = likelihood(X, y, f, chain.beta(i), est.sigma, est.theta, R, R_inv, nugget);
    fprintf(fd, "%.10e,%.10f\n", likeli.beta(i), chain.beta(i));
  endfor
  fclose(fd);

  # calc the sigma likelihood
  fd = fopen([dir, "/", prefix, "-chain-sigma-likelihood.dat"], "wt");
  fprintf(fd, "likelihood_sigma,sigma\n");
  for i = 1:n
    likeli.sigma(i) = likelihood(X, y, f, est.beta, chain.sigma(i), est.theta, R, R_inv, nugget);
    fprintf(fd, "%.10e,%.10f\n", likeli.sigma(i), chain.sigma(i));
  endfor
  fclose(fd);
  
  # calc the theta likelihoods
  for j = 1:columns(chain.theta)
    file = [dir, "/", prefix, "-chain-theta", int2str(j), "-likelihood.dat"];
    fd = fopen(file, "wt");
    fprintf(fd, "likelihood_theta%d,theta%d\n", j,j);
    for i = 1:n
      theta = est.theta;
      theta(j) = chain.theta(i,j);
      R = scf_gaussianm(X, theta, nugget);
      R_inv = R^-1;
      likeli.theta(i,j) = likelihood(X, y, f, est.beta, est.sigma, theta, R, R_inv, nugget);
      fprintf(fd, "%.10e,%.10f\n", likeli.theta(i,j), chain.theta(i,j));
    endfor
    fclose(fd);
  endfor
  
endfunction
