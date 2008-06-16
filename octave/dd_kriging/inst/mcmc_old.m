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


## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function chain = mcmc_mh(s1, x0_beta, x0_theta, x0_sigma, X, y, n)
  if (nargin != 7)
    usage("mcmc_mh(s1, x0_beta, x0_theta, x0_sigma, X, y, n)");
  endif

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  f = ones(rows(X), 1);

  # initialise the x values
  x.beta = 0.0;
  x_old.beta = x0_beta;

  x.theta = zeros(1, columns(X));
  x_old.theta = x0_theta;

  x.sigma = 0.0;
  x_old.sigma = x0_sigma;

  pi_old = 1 / x0_sigma^2;

  # "krig_likelihood(sigma, theta, X, y, beta, f)"
  q_old = krig_likelihood(x0_sigma, x0_theta, X, y, x0_beta, f);

  accepted = 0;
  s1 = s1^2;

  C = cov(X);
  theta_var = diag(C);
  
  # initialise the chain
  chain_beta = [];
  chain_theta = [];
  chain_sigma = [];

  for i = 1:n
    # step 1: generate x from the proposal distribution
    x.sigma = normrnd(x_old.sigma, s1);
    
    # sigma has to be positive
    while (x.sigma < 0)
      x.sigma = normrnd(x_old.sigma, s1);
    endwhile

    for i = 1:columns(x.beta)
      x.beta(i) = normrnd(x_old.beta(i), s1);
    endfor
    for i = 1:columns(x.theta)
      x.theta(i) = normrnd(x_old.theta(i), theta_var(i));

      # theta has to be positive
      while ((x.theta(i) < 0) || (x.theta(i) > 5))
	x.theta(i) = normrnd(x_old.theta(i), theta_var(i));
      endwhile
    endfor

    # step 2: calcuate the probability of move
    u = rand();
    pi_new = 1 / x.sigma^2;
    q_new = krig_likelihood(x.sigma, x.theta, X, y, x.beta, f);

    ratio = (pi_new * q_old) / (pi_old * q_new);

    # step 3: accept, if u < alpha(x, x')
    if u <= min(ratio, 1)
      accepted++;
      pi_old = pi_new;
      q_old = q_new;
      x_old.beta = x.beta;
      x_old.theta = x.theta;
      x_old.sigma = x.sigma;

      # put results into markov chain
      chain_beta = [chain_beta, x_old.beta];
      chain_theta = [chain_theta; x_old.theta];
      chain_sigma = [chain_sigma, x_old.sigma];

      chain.accepted = accepted;
      chain.beta = chain_beta;
      chain.sigma = chain_sigma;
      chain.theta = chain_theta;
    endif
  endfor

endfunction