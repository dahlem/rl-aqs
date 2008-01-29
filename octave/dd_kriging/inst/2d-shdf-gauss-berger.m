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

function y = shdf(x1, x2)
  if ((x1 < -2) || (x1 > 2))
    error("x1 of the shcf function is defined over [-2;2]");
  endif
  if ((x2 < -1) || (x2 > 1))
    error("x2 of the shcf function is defined over [-1;1]");
  endif
  
  y = (4 - 2.1 * x1^2 + x1^4/3) * x1^2 + x1 * x2 + (-4 + 4 * x2^2) * x2^2;
endfunction


function mean_chain = calc_mean(chain)
  mean_chain.beta = [];
  mean_chain.theta1 = [];
  mean_chain.theta2 = [];
  mean_chain.sigma = [];

  for i = 1:chain.accepted
    mean_chain.beta = [mean_chain.beta, mean(chain.beta(1:i))];
    mean_chain.theta1 = [mean_chain.theta1, mean(chain.theta(:,1)(1:i))];
    mean_chain.theta2 = [mean_chain.theta2, mean(chain.theta(:,2)(1:i))];
    mean_chain.sigma = [mean_chain.sigma, mean(chain.sigma(1:i))];
  endfor
endfunction


function chain = shdf_krig()
  global X;
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  y = [];
  
  for i = 1:columns(x1_vec)
    y = [y; shdf(x1_vec(i), x2_vec(i))];
  endfor
  
  # mcmc_mh(s1, x0_theta, X, y, n)
  chain = mcmc_mh(2, [1, 1], X, y, 20000, "Berger");

  mean_chain = calc_mean(chain);
  
  start = chain.accepted - 10000;

  burn.beta = chain.beta(start:end);
  burn.sigma = chain.sigma(start:end);
  burn.theta1 = chain.theta(:,1)(start:end);
  burn.theta2 = chain.theta(:,2)(start:end);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate.dat";
  samples = 100;

  sampled.beta = burn.beta(1:3:end);
  sampled.sigma = burn.sigma(1:3:end);
  sampled.theta1 = burn.theta1(1:3:end);
  sampled.theta2 = burn.theta2(1:3:end);
  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  f = ones(rows(X), 1);

  theta = [];
  theta(1) = mean(sampled.theta1);
  theta(2) = mean(sampled.theta2);

  beta = mean(sampled.beta);
  
  R = scf_gaussianm(X, theta);

  fd = fopen(outfile, "wt");
  for x1 = x1_vec
    for x2 = x2_vec
      fprintf(fd, "%14.6f %14.6f %14.6f\n", x1, x2, krig(X, [x1, x2], R, beta, theta, y, f)(1));
    endfor
    fprintf(fd, "\n");
  endfor
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-beta.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", mean_chain.beta);
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-sigma.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", mean_chain.sigma);
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-theta1.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", mean_chain.theta1);
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-theta2.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", mean_chain.theta2);
  fclose(fd);
  
  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-beta-sampled.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", sampled.beta);
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-sigma-sampled.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", sampled.sigma);
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-theta1-sampled.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", sampled.theta1);
  fclose(fd);

  outfile = "../doc/2d/shdf/gauss/berger/2d-shdf-surrogate-theta2-sampled.dat";
  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f\n", sampled.theta2);
  fclose(fd);
  
endfunction


function test_surrogate(X, R, beta, theta, y, f)
  for i = 1:rows(X)
    resK = krig(X, [X(i,1), X(i,2)], R, beta, theta, y, f);
    resR = shdf(X(i,1), X(i,2));

    if (abs(resK(1) - resR) > 0.00001)
      warning("The surrogate model did not return a correct result");
    endif
  endfor
endfunction
