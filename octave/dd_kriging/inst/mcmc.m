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


## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function chain = mcmc_mh(scale, x0_theta, X, y, n, prior = "Jeffrey", nugget = 0)
##  if (nargin < 6 || nargin > 7)
##    usage("mcmc_mh(scale, x0_theta, X, y, n, prior)");
##  endif

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

  ## initialise the x values
  x.theta = zeros(1, columns(X));
  x_old.theta = x0_theta;

  x.beta = 0.0;
  x.sigma = 0.0;

  ## "krig_likelihood(sigma, theta, X, y, beta, f)"
  [q_old, x_old.beta, x_old.sigma] = krig_likelihood(x0_theta, X, y, f, nugget);

  ## if the berger prior is required transform sigma
  if (strcmp(prior, "Berger"))
    x_old.sigma = bergerPrior(x_old.sigma);
  endif
  
  pi_old = 1 / x_old.sigma;

##  C = cov(X);
##  theta_var = diag(C)';
  
  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(1, n);
  chain.theta = zeros(n, 2);
  chain.sigma = zeros(1, n);
  chain.l = zeros(1, n);

  for i = 1:n
   ## step 1: generate x from the proposal distribution
    ## theta has to be positive
    for k = 1:columns(X)
##      do
##      a = gamma_rnd(scale, scale)
##      1/sqrt(a)
##        x.theta(k) = 1/sqrt(a);
##      until (x.theta(k) < 5)
      x.theta(k) = prior_pick(5, scale, columns(X));
      x.theta(k) = e.^x.theta(k);
    endfor
    ## step 2: calcuate the probability of move
    [q_new, x.beta, x.sigma] = krig_likelihood(x.theta, X, y, f, nugget);

    ## if the berger prior is required transform sigma
    if (strcmp(prior, "Berger"))
      x.sigma = bergerPrior(x.sigma);
    endif
  
    pi_new = 1 / x.sigma;

    ratio = (pi_new * q_old) / (pi_old * q_new);

    ## step 3: accept, if u < alpha(x, x')
    u = rand();
    
    if u <= min(ratio, 1)
      ## reject also above a certain threshold
      chain.accepted++;
      pi_old = pi_new;
      q_old = q_new;
      x_old.beta = x.beta;
      x_old.theta = x.theta;
      x_old.sigma = x.sigma;

      ## put results into markov chain
      chain.beta(chain.accepted) = x_old.beta;
      chain.theta(chain.accepted, :) = x_old.theta;
      chain.sigma(chain.accepted) = x_old.sigma;
      chain.l(chain.accepted) = q_old;
    endif
  endfor

  chain.beta = chain.beta(1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.l = chain.l(1:chain.accepted);
  chain.theta = chain.theta(1:chain.accepted,:);
endfunction


function berger = bergerPrior(sigma)
  berger = sqrt(sigma)^3;
endfunction


## -------------------------------------------------
## From "A nonstationary covariance based kriging method for
## metamodeling in engineering design" by Xiong et. al.
## -------------------------------------------------
function chain = mcmc_nonst_mh(scale, ub, x0_eta, X, y, n, prior="Jeffrey", xi, nugget=0)
  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (!strcmp(prior, "Jeffrey") && !strcmp(prior, "Berger"))
    error("The given prior is not supported");
  endif

  f = ones(rows(X), 1);
  k = rows(xi);
  l = columns(xi);

  ## initialise the x values
  x_old.Eta = x0_eta;

  x.Eta = zeros(k, l);
  x.beta = 0.0;
  x.sigma = 0.0;
  x.sigma = 0.0;

  ## "krig_likelihood(sigma, theta, X, y, beta, f)"
  [q_old, x_old.beta, x_old.sigma] = krig_nonst_likelihood(X, y, f, xi, x_old.Eta, nugget);

  ## if the berger prior is required transform sigma
  if (strcmp(prior, "Berger"))
    x_old.sigma = bergerPrior(x_old.sigma);
  endif
  
  pi_old = 1 / x_old.sigma;

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(1, n);
  chain.Eta = zeros(k, l, n);
  chain.sigma = zeros(1, n);
  chain.l = zeros(1, n);

  for i = 1:n
    ## step 1: generate x from the proposal distribution
    for r = 1:rows(x.Eta)
      for c = 1:columns(x.Eta)
	x.Eta(r,c) = prior_pick(ub, scale, columns(X));
	x.Eta(r,c) = e.^x.Eta(r,c);
      endfor
    endfor

    ## step 2: calcuate the probability of move
    [q_new, x.beta, x.sigma] = krig_nonst_likelihood(X, y, f, xi, x.Eta, nugget);

    ## if the berger prior is required transform sigma
    if (strcmp(prior, "Berger"))
      x.sigma = bergerPrior(x.sigma);
    endif
  
    pi_new = 1 / x.sigma;

    ratio = (pi_new * q_old) / (pi_old * q_new);

    ## step 3: accept, if u < alpha(x, x')
    u = rand();
    
    if u <= min(ratio, 1)
      ## reject also above a certain threshold
      chain.accepted++;
      pi_old = pi_new;
      q_old = q_new;
      x_old.beta = x.beta;
      x_old.Eta = x.Eta;
      x_old.sigma = x.sigma;

      ## put results into markov chain
      chain.beta(chain.accepted) = x_old.beta;
      chain.sigma(chain.accepted) = x_old.sigma;
      chain.l(chain.accepted) = q_old;

      for a = 1:rows(x_old.Eta)
	for b = 1:columns(x_old.Eta)
	  chain.Eta(a,b,chain.accepted) = x_old.Eta(a,b);
	endfor
      endfor
    endif
  endfor

  chain.beta = chain.beta(1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.Eta = chain.Eta(:,:,1:chain.accepted);
  chain.l = chain.l(1:chain.accepted);
endfunction
