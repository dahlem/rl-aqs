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
function chain = mcmc_mh(scale, x0_theta, X, y, n, prior = "Jeffrey")
  if (nargin < 5 || nargin > 6)
    usage("mcmc_mh(scale, x0_theta, X, y, n, prior)");
  endif

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  if (!strcmp(prior, "Jeffrey") && !strcmp(prior, "Berger"))
    error("The given prior is not supported");
  endif

  f = ones(rows(X), 1);

  # initialise the x values
  x.theta = zeros(1, columns(X));
  x_old.theta = x0_theta;

  x.beta = 0.0;
  x.sigma = 0.0;

  # "krig_likelihood(sigma, theta, X, y, beta, f)"
  [q_old, x_old.beta, x_old.sigma] = krig_likelihood(x0_theta, X, y, f);

  # if the berger prior is required transform sigma
  if (strcmp(prior, "Berger"))
    x_old.sigma = bergerPrior(x_old.sigma);
  endif
  
  pi_old = 1 / x_old.sigma;

  C = cov(X);
  theta_var = diag(C);
  
  # initialise the chain
  chain.accepted = 0;
  chain.beta = [];
  chain.theta = [];
  chain.sigma = [];

  for i = 1:n
    # step 1: generate x from the proposal distribution

    for i = 1:columns(x.theta)
      x.theta(i) = normrnd(x_old.theta(i), scale * theta_var(i));

      # theta has to be positive
      while ((x.theta(i) < 0) || (x.theta(i) > 5))
	x.theta(i) = normrnd(x_old.theta(i), scale * theta_var(i));
      endwhile
    endfor

    # step 2: calcuate the probability of move
    [q_new, x.beta, x.sigma] = krig_likelihood(x.theta, X, y, f);

    # if the berger prior is required transform sigma
    if (strcmp(prior, "Berger"))
      x.sigma = bergerPrior(x.sigma);
    endif
  
    pi_new = 1 / x.sigma;

    ratio = (pi_new * q_old) / (pi_old * q_new);

    # step 3: accept, if u < alpha(x, x')
    u = rand();
    
    if u <= min(ratio, 1)
      chain.accepted++;
      pi_old = pi_new;
      q_old = q_new;
      x_old.beta = x.beta;
      x_old.theta = x.theta;
      x_old.sigma = x.sigma;

      # put results into markov chain
      chain.beta = [chain.beta, x_old.beta];
      chain.theta = [chain.theta; x_old.theta];
      chain.sigma = [chain.sigma, x_old.sigma];
    endif
  endfor

endfunction


function berger = bergerPrior(sigma)
  berger = sqrt(sigma)^3;
endfunction
