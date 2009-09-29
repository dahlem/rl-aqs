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

function x_bar_new = welford_mean(n, x_bar, x)
  d = x - x_bar;
  x_bar_new = x_bar + d / n;
endfunction

## squashing function with a minimum value a and a maximum value b
function f = logit(theta, a, b)
  f = log(theta-a) - log(b-theta);
endfunction

## inverse squashing function with a minimum value a and a maximum value b
function fi =  logitinv(z, a, b)
  fi = b-(b-a)./(1+exp(z));
endfunction

## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function [chain, acceptanceRate] = \
      mcmc_mhgibbsH(x0_theta, x0_beta, x0_sigma, X, y, batchlength, batches, \
                    p_tuning = 1.0, p_acceptance = 0.23, aTheta = 3/500, bTheta = 1, \
                    aSigma, bSigma, nugget = 0, FUN = @(x) 1, verbose = 0)

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  samples = batches * batchlength;
  nTheta = columns(x0_theta);
  nBeta = columns(x0_beta);
  dims = nBeta + 1 + nTheta;
  F = FUN(X);

  ## initialise the x values
  x.theta = logit(x0_theta, aTheta, bTheta);
  x.beta = x0_beta;
  x.sigma = log(x0_sigma);
  theta = [x.beta, x.sigma, x.theta];

  ## tuning for the thetas
  tuning = zeros(1, dims);
  ## batch acceptance rate
  batchaccept = zeros(1, dims);
  acceptanceRate = zeros(batches, dims);

  ## temporary accepted values
  pi_accept = q_accept = beta_accept = theta_accept = sigma_accept = [];

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(nBeta, samples);
  chain.theta = zeros(samples, nTheta);
  chain.sigma = zeros(1, samples);
  chain.l = zeros(1, samples);

  ## init the batch variables for adaptive tuning
  for i = 1:dims
    tuning(i) = p_tuning;
    batchaccept(i) = 0;
    tuning(i) = log(tuning(i));
  endfor

  q_old = logLikeli(theta(1:nBeta)', theta(nBeta+2:nTheta+nBeta+1), \
                    aTheta, bTheta, theta(nBeta+1), aSigma, bSigma, X, \
                    y, F, nugget);

  for b = 0:(batches-1)
    for i = 1:batchlength
      ## gibbs steps
      for thetas = 1:dims
        thetaj = theta(thetas);
        theta(thetas) = normrnd(theta(thetas), exp(tuning(thetas)));

        ## step 2: calcuate the probability of move
        q_new = logLikeli(theta(1:nBeta)', theta(nBeta+2:nTheta+nBeta+1), \
                          aTheta, bTheta, theta(nBeta+1), aSigma, bSigma, X, \
                          y, F, nugget);

        ratio = q_new - q_old;

        ## step 3: accept, if u < alpha(x, x')
        u = rand();
        if u <= exp(ratio)
          batchaccept(thetas)++;
          q_accept = q_new;
          beta_accept = theta(1:nBeta)';
          theta_accept = theta(nBeta+2:nTheta+nBeta+1);
          sigma_accept = theta(nBeta+1);
          q_old = q_new;
        else
          x.theta(thetas) = thetaj;
        endif
      endfor

      ## put results into markov chain
      chain.accepted++;
      chain.beta(:, chain.accepted) = beta_accept;
      chain.theta(chain.accepted, :) = logitinv(theta_accept, aTheta, bTheta);
      chain.sigma(chain.accepted) = sigma_accept;
      chain.l(chain.accepted) = q_accept;
    endfor

    ## adaptive tuning
    acceptanceRate(b+1,:) = batchaccept/batchlength;
    for j = 1:dims
      if (batchaccept(j)/batchlength > p_acceptance)
        tuning(j) += min(0.01, 1.0/sqrt(b+1));
      else
        tuning(j) -= min(0.01, 1.0/sqrt(b+1));
      endif
      batchaccept(j) = 0;
    endfor

    if (verbose)
      fprintf(stdout, "Batch: %d of %d\n", b, batches);
      fprintf(stdout, "Metropolis batch acceptance rate:\n");

      for j = 1:dims
        fprintf(stdout, "   %1.3f   ", acceptanceRate(b+1, j));
      endfor
      fprintf(stdout, "\n");
      for j = 1:dims
        fprintf(stdout, "+/-%1.3f   ", tuning(j));
      endfor
      fprintf(stdout, "\n\n");
      fflush(stdout);
    endif
  endfor
endfunction


## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function chain = mcmc_mh(a1, x0_theta, X, y, n, prior = "Jeffrey", \
                         nugget = 0, FUN = @(x) 1)
  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  if (!strcmp(prior, "Jeffrey") && !strcmp(prior, "Berger"))
    error("The given prior is not supported");
  endif

  dims = columns(X);
  F = FUN(X);

  ## initialise the x values
  x.theta = zeros(1, dims);
  x_old.theta = x0_theta;

  x.beta = zeros(columns(F), 1);
  x.sigma = 0.0;

  ## "krig_likelihood(sigma, theta, X, y, beta, f)"
  [q_old, x_old.beta, x_old.sigma] = krig_likelihood(x0_theta, X, y, F, nugget);

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(columns(F), n);
  chain.theta = zeros(n, dims);
  chain.sigma = zeros(1, n);
  chain.l = zeros(1, n);

  mean.thetaP = x0_theta.^-2;
##  pi_old = x_old.sigma^-2;
  pi_old = mean.thetaP;

  ## if the berger prior is required transform sigma
  if (strcmp(prior, "Berger"))
    mean.thetaP = bergerPrior(mean.thetaP);
  endif

  for i = 1:n
##    x.theta = gamrnd(a1/2, a1/2 * mean.thetaP);
    x.theta = gamrnd(a1/2, a1/2 * x0_theta.^-2);
    ## step 1: generate x from the proposal distribution
    ## theta has to be positive
##    for k = 1:dims
##      if (chain.accepted == 0)
##      x.theta(k) = prior_pick(1./x0_theta(k).^2, scale, columns(X));
##      else
##      x.theta(k) = prior_pick(mean.thetaP(k), scale, columns(X));
##      endif
##      x.theta(k) = e^x.theta(k);
##    endfor

    ## step 2: calcuate the probability of move
    [q_new, x.beta, x.sigma] = krig_likelihood(x.theta, X, y, F, nugget);

##    pi_new = x.sigma^-2;
    pi_new = x.theta.^-2;

    ratio = (q_new / q_old) * (pi_old / pi_new);
##    ratio = (pi_new * q_old) / (pi_old * q_new);

    ## step 3: accept, if u < alpha(x, x')
    if rand() <= min(ratio, 1)
      ## reject also above a certain threshold
      chain.accepted++;
      pi_old = pi_new;
      q_old = q_new;
      x_old.beta = x.beta;
      x_old.theta = x.theta;
      x_old.sigma = x.sigma;

      ## put results into markov chain
      chain.beta(:, chain.accepted) = x_old.beta;
      chain.theta(chain.accepted, :) = x_old.theta;
      chain.sigma(chain.accepted) = x_old.sigma;
      chain.l(chain.accepted) = q_old;
      mean.thetaP = x.theta.^-2; ##welford_mean(i, mean.thetaP, 1./x.theta.^2);

      ## if the berger prior is required transform sigma
      if (strcmp(prior, "Berger"))
        mean.thetaP = bergerPrior(mean.thetaP);
      endif
    endif
  endfor

  chain.beta = chain.beta(:, 1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.l = chain.l(1:chain.accepted);
  chain.theta = chain.theta(1:chain.accepted,:);
endfunction


## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function [chain, acceptanceRate] = \
      mcmc_mhgibbs(x0_theta, X, y, batchlength, batches, \
                   p_tuning = 1.0, p_acceptance = 0.23, aTheta = 3/500, bTheta = 1, \
                   nugget = 0, FUN = @(x) 1, verbose = 0)

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  samples = batches * batchlength;
  dims = columns(X);
  F = FUN(X);

  ## initialise the x values
  x.theta = logit(x0_theta, aTheta, bTheta);

  ## tuning for the thetas
  tuning = zeros(1, dims);
  ## batch acceptance rate
  batchaccept = zeros(1, dims);
  acceptanceRate = zeros(batches, dims);

  ## temporary accepted values
  pi_accept = q_accept = beta_accept = theta_accept = sigma_accept = [];

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(columns(F), samples);
  chain.theta = zeros(samples, dims);
  chain.sigma = zeros(1, samples);
  chain.l = zeros(1, samples);

  ## init the batch variables for adaptive tuning
  for i = 1:dims
    tuning(i) = p_tuning;
    batchaccept(i) = 0;
    tuning(i) = log(tuning(i));
  endfor

  [q_old, x.beta, x.sigma] = krig_likelihood(logitinv(x.theta, aTheta, bTheta), X, y, F, nugget);
  phi_cand = logitinv(x.theta, aTheta, bTheta);
  pi_old = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand));

  for b = 0:(batches-1)
    for i = 1:batchlength
      ## gibbs steps
      for thetas = 1:dims
        thetaj = x.theta(thetas);
        x.theta(thetas) = normrnd(x.theta(thetas), exp(tuning(thetas)));

        ## step 2: calcuate the probability of move
        [q_new, x.beta, x.sigma] = krig_likelihood(logitinv(x.theta, aTheta, bTheta), X, y, F, nugget);
        phi_cand = logitinv(x.theta, aTheta, bTheta);
        pi_new = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand));

##      ratio = (q_new / q_old) * (pi_old / pi_new);
        ratio = (q_new + pi_new) - (q_old + pi_old);
        ##    ratio = (pi_new * q_old) / (pi_old * q_new);

        ## step 3: accept, if u < alpha(x, x')
##      if rand() <= min(ratio, 1)
        ## step 3: accept, if u < alpha(x, x')
        u = rand();
        if u <= exp(ratio)
          batchaccept(thetas)++;
          q_accept = q_new;
          beta_accept = x.beta;
          theta_accept = x.theta;
          sigma_accept = x.sigma;
          q_old = q_new;
          pi_old = pi_new;
        else
          x.theta(thetas) = thetaj;
        endif
      endfor

      ## put results into markov chain
      chain.accepted++;
      chain.beta(:, chain.accepted) = beta_accept;
      chain.theta(chain.accepted, :) = logitinv(theta_accept, aTheta, bTheta);
      chain.sigma(chain.accepted) = sigma_accept;
      chain.l(chain.accepted) = q_accept;
    endfor

    ## adaptive tuning
    acceptanceRate(b+1,:) = batchaccept/batchlength;
    for j = 1:dims
      if (batchaccept(j)/batchlength > p_acceptance)
        tuning(j) += min(0.01, 1.0/sqrt(b+1));
      else
        tuning(j) -= min(0.01, 1.0/sqrt(b+1));
      endif
      batchaccept(j) = 0;
    endfor

    if (verbose)
      fprintf(stdout, "Batch: %d of %d\n", b, batches);
      fprintf(stdout, "Metropolis batch acceptance rate:\n");

      for j = 1:dims
        fprintf(stdout, "   %1.3f   ", acceptanceRate(b+1, j));
      endfor
      fprintf(stdout, "\n");
      for j = 1:dims
        fprintf(stdout, "+/-%1.3f   ", tuning(j));
      endfor
      fprintf(stdout, "\n\n");
      fflush(stdout);
    endif
  endfor
endfunction


## add log transforms to the mcmc target
## add the log transform elements to the mcmc target
## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function [chain, acceptanceRate] = \
      mcmc_mhgibbsStochastic(x0_theta, X, y, C, x0_sigma, batchlength, batches, \
                   p_tuning, p_acceptance = 0.23, aTheta = 3/500, bTheta = 1, \
                   nugget = 0, FUN = @(x) 1, verbose = 0, p=2)

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  samples = batches * batchlength;
  dims = columns(X) + 1;
  F = FUN(X);

  ## initialise the x values
  x.theta = logit(x0_theta, aTheta, bTheta);
  n = rows(X);
  x.sigma = log(x0_sigma);
  
  ## tuning for the thetas
  tuning = zeros(1, dims);
  ## batch acceptance rate
  batchaccept = zeros(1, dims);
  acceptanceRate = zeros(batches, dims);

  ## temporary accepted values
  pi_accept = q_accept = beta_accept = theta_accept = sigma_accept = [];

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(columns(F), samples);
  chain.theta = zeros(samples, length(x.theta));
  chain.sigma = zeros(1, samples);
  chain.l = zeros(1, samples);

  ## init the batch variables for adaptive tuning
  tuning = log(p_tuning);
  for i = 1:dims
    batchaccept(i) = 0;
  endfor

  [q_old, x.beta] = krig_likelihoodStochastic(logitinv(x.theta, aTheta, bTheta),C,X, y, F, exp(x.sigma),nugget,p);
  phi_cand = logitinv(x.theta, aTheta, bTheta);
  pi_old = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand)) + x.sigma;

  for b = 0:(batches-1)
    for i = 1:batchlength
      ## gibbs steps
      for thetas = 1:length(x.theta)
        thetaj = x.theta(thetas);
        x.theta(thetas) = normrnd(x.theta(thetas), exp(tuning(thetas)));

        ## step 2: calcuate the probability of move
        [q_new, x.beta] = krig_likelihoodStochastic(logitinv(x.theta, aTheta, bTheta),C,X, y, F, exp(x.sigma),nugget,p);
        phi_cand = logitinv(x.theta, aTheta, bTheta);
        pi_new = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand)) + x.sigma;

##      ratio = (q_new / q_old) * (pi_old / pi_new);
        ratio = (q_new + pi_new) - (q_old + pi_old);
        ##    ratio = (pi_new * q_old) / (pi_old * q_new);

        ## step 3: accept, if u < alpha(x, x')
##      if rand() <= min(ratio, 1)
        ## step 3: accept, if u < alpha(x, x')
        u = rand();
        if u <= exp(ratio)
          batchaccept(thetas)++;
          q_accept = q_new;
          beta_accept = x.beta;
          theta_accept = x.theta;
          sigma_accept = x.sigma;
          q_old = q_new;
          pi_old = pi_new;
        else
          x.theta(thetas) = thetaj;
        endif
      endfor

      sigmaj = x.sigma;
      x.sigma = gamrnd(exp(tuning(dims)), x.sigma / exp(tuning(dims)));

      ## step 2: calcuate the probability of move
      [q_new, x.beta] = krig_likelihoodStochastic(logitinv(x.theta, aTheta, bTheta), C,X, y, F, exp(x.sigma),nugget,p);
      phi_cand = logitinv(x.theta, aTheta, bTheta);
      pi_new = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand)) + x.sigma;

      ##      ratio = (q_new / q_old) * (pi_old / pi_new);
      ratio = (q_new + pi_new) - (q_old + pi_old);
      ##    ratio = (pi_new * q_old) / (pi_old * q_new);

      ## step 3: accept, if u < alpha(x, x')
      ##      if rand() <= min(ratio, 1)
      ## step 3: accept, if u < alpha(x, x')
      u = rand();
      if u <= exp(ratio)
        batchaccept(dims)++;
        q_accept = q_new;
        beta_accept = x.beta;
        sigma_accept = x.sigma;
        q_old = q_new;
        pi_old = pi_new;
      else
        x.sigma = sigmaj;
      endif

      ## put results into markov chain
      chain.accepted++;
      chain.beta(:, chain.accepted) = beta_accept;
      chain.theta(chain.accepted, :) = logitinv(theta_accept, aTheta, bTheta);
      chain.sigma(chain.accepted) = exp(sigma_accept);
      chain.l(chain.accepted) = q_accept;
    endfor

    ## adaptive tuning
    acceptanceRate(b+1,:) = batchaccept/batchlength;
    for j = 1:length(x0_theta)
      if (batchaccept(j)/batchlength > p_acceptance)
        tuning(j) += min(0.01, 1.0/sqrt(b+1));
      else
        tuning(j) -= min(0.01, 1.0/sqrt(b+1));
      endif
      batchaccept(j) = 0;
    endfor
    if (batchaccept(dims)/batchlength > p_acceptance)
      tuning(dims) -= min(0.01, 1.0/sqrt(b+1));
    else
      tuning(dims) += min(0.01, 1.0/sqrt(b+1));
    endif
    batchaccept(dims) = 0;

    if (verbose)
      fprintf(stdout, "Batch: %d of %d\n", b, batches);
      fprintf(stdout, "Metropolis batch acceptance rate:\n");

      for j = 1:dims
        fprintf(stdout, "   %1.3f   ", acceptanceRate(b+1, j));
      endfor
      fprintf(stdout, "\n");
      for j = 1:dims
        fprintf(stdout, "+/-%1.3f   ", tuning(j));
      endfor
      fprintf(stdout, "\n\n");
      fflush(stdout);
    endif
  endfor
endfunction

## add log transforms to the mcmc target
## add the log transform elements to the mcmc target
## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function [chain, acceptanceRate] = \
      mcmc_mhgibbsBanerjee(x0_theta, X, y, batchlength, batches, \
                   p_tuning = 1.0, p_acceptance = 0.23, aTheta = 3/500, bTheta = 1, \
                   nugget = 0, FUN = @(x) 1, verbose = 0)

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  samples = batches * batchlength;
  dims = columns(X);
  F = FUN(X);

  ## initialise the x values
  x.theta = logit(x0_theta, aTheta, bTheta);

  ## tuning for the thetas
  tuning = zeros(1, dims);
  ## batch acceptance rate
  batchaccept = zeros(1, dims);
  acceptanceRate = zeros(batches, dims);

  ## temporary accepted values
  pi_accept = q_accept = beta_accept = theta_accept = sigma_accept = [];

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(columns(F), samples);
  chain.theta = zeros(samples, dims);
  chain.sigma = zeros(1, samples);
  chain.l = zeros(1, samples);

  ## init the batch variables for adaptive tuning
  for i = 1:dims
    tuning(i) = p_tuning;
    batchaccept(i) = 0;
    tuning(i) = log(tuning(i));
  endfor

  [q_old, x.beta, x.sigma] = krig_likelihoodBanerjee(logitinv(x.theta, aTheta, bTheta), aTheta, bTheta,X, y, F, nugget);
  phi_cand = logitinv(x.theta, aTheta, bTheta);
  pi_old = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand));

  for b = 0:(batches-1)
    for i = 1:batchlength
      ## gibbs steps
      for thetas = 1:dims
        thetaj = x.theta(thetas);
        x.theta(thetas) = normrnd(x.theta(thetas), exp(tuning(thetas)));

        ## step 2: calcuate the probability of move
        [q_new, x.beta, x.sigma] = krig_likelihoodBanerjee(logitinv(x.theta, aTheta, bTheta), aTheta, bTheta,X, y, F, nugget);
        phi_cand = logitinv(x.theta, aTheta, bTheta);
        pi_new = sum(log(phi_cand - aTheta) + log(bTheta - phi_cand));

##      ratio = (q_new / q_old) * (pi_old / pi_new);
        ratio = (q_new + pi_new) - (q_old + pi_old);
        ##    ratio = (pi_new * q_old) / (pi_old * q_new);

        ## step 3: accept, if u < alpha(x, x')
##      if rand() <= min(ratio, 1)
        ## step 3: accept, if u < alpha(x, x')
        u = rand();
        if u <= exp(ratio)
          batchaccept(thetas)++;
          q_accept = q_new;
          beta_accept = x.beta;
          theta_accept = x.theta;
          sigma_accept = x.sigma;
          q_old = q_new;
          pi_old = pi_new;
        else
          x.theta(thetas) = thetaj;
        endif
      endfor

      ## put results into markov chain
      chain.accepted++;
      chain.beta(:, chain.accepted) = beta_accept;
      chain.theta(chain.accepted, :) = logitinv(theta_accept, aTheta, bTheta);
      chain.sigma(chain.accepted) = sigma_accept;
      chain.l(chain.accepted) = q_accept;
    endfor

    ## adaptive tuning
    acceptanceRate(b+1,:) = batchaccept/batchlength;
    for j = 1:dims
      if (batchaccept(j)/batchlength > p_acceptance)
        tuning(j) += min(0.01, 1.0/sqrt(b+1));
      else
        tuning(j) -= min(0.01, 1.0/sqrt(b+1));
      endif
      batchaccept(j) = 0;
    endfor

    if (verbose)
      fprintf(stdout, "Batch: %d of %d\n", b, batches);
      fprintf(stdout, "Metropolis batch acceptance rate:\n");

      for j = 1:dims
        fprintf(stdout, "   %1.3f   ", acceptanceRate(b+1, j));
      endfor
      fprintf(stdout, "\n");
      for j = 1:dims
        fprintf(stdout, "+/-%1.3f   ", tuning(j));
      endfor
      fprintf(stdout, "\n\n");
      fflush(stdout);
    endif
  endfor
endfunction


## This function implements the metropolis hastings algorithm for
## estimating the model parameters of the bayesian likelihood equation,
## where s1 is the constant standard deviation, x0_beta is an initial
## values of beta, x0_theta is a vector of initial values for theta,
## x0_sigma is the initial value for sigma, X is the matrix of n
## observations (rows) with d dimensions (columns), y is the output
## vector at the n observations, and n is the number of iterations for
## the monte carlo sampling.
function [chain, acceptanceRate] = \
      mcmc_mhgibbsGamma(x0_theta, X, y, batchlength, batches, \
                        p_tuning = 1.0, p_acceptance = 0.23, a1, \
                        nugget = 0, FUN = @(x) 1, verbose = 0)

  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (columns(x0_theta) != columns(X))
    error("The vector x0_theta has to have the same dimension as matrix columns.");
  endif

  samples = batches * batchlength;
  dims = columns(X);
  F = FUN(X);

  ## initialise the x values
  x.theta = log(x0_theta);

  ## tuning for the thetas
  tuning = zeros(1, dims);
  ## batch acceptance rate
  batchaccept = zeros(1, dims);
  acceptanceRate = zeros(batches, dims);

  ## temporary accepted values
  pi_accept = q_accept = beta_accept = theta_accept = sigma_accept = [];

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(columns(F), samples);
  chain.theta = zeros(samples, dims);
  chain.sigma = zeros(1, samples);
  chain.l = zeros(1, samples);

  ## init the batch variables for adaptive tuning
  for i = 1:dims
    tuning(i) = p_tuning;
    batchaccept(i) = 0;
    tuning(i) = log(tuning(i));
  endfor

  [q_old, x.beta, x.sigma] = krig_likelihood(exp(x.theta), X, y, F, nugget);
  pi_old = sum(gamrnd(a1/2, a1/2 * x.theta.^-2));

  for b = 0:(batches-1)
    for i = 1:batchlength
      ## gibbs steps
      for thetas = 1:dims
        thetaj = x.theta(thetas);
        x.theta(thetas) = normrnd(x.theta(thetas), exp(tuning(thetas)));

        ## step 2: calcuate the probability of move
        [q_new, x.beta, x.sigma] = krig_likelihood(exp(x.theta), X, y, F, nugget);
	pi_new = sum(gamrnd(a1/2, a1/2 * x.theta.^-2));

        ratio = (q_new * pi_old) / (q_old * pi_new);
        ## step 3: accept, if u < alpha(x, x')
        u = rand();
        if u <= exp(ratio)
          batchaccept(thetas)++;
          q_accept = q_new;
          beta_accept = x.beta;
          theta_accept = x.theta;
          sigma_accept = x.sigma;
          q_old = q_new;
          pi_old = pi_new;
        else
          x.theta(thetas) = thetaj;
        endif
      endfor

      ## put results into markov chain
      chain.accepted++;
      chain.beta(:, chain.accepted) = beta_accept;
      chain.theta(chain.accepted, :) = exp(theta_accept);
      chain.sigma(chain.accepted) = sigma_accept;
      chain.l(chain.accepted) = q_accept;
    endfor

    ## adaptive tuning
    acceptanceRate(b+1,:) = batchaccept/batchlength;
    for j = 1:dims
      if (batchaccept(j)/batchlength > p_acceptance)
        tuning(j) += min(0.01, 1.0/sqrt(b+1));
      else
        tuning(j) -= min(0.01, 1.0/sqrt(b+1));
      endif
      batchaccept(j) = 0;
    endfor

    if (verbose)
      fprintf(stdout, "Batch: %d of %d\n", b, batches);
      fprintf(stdout, "Metropolis batch acceptance rate:\n");

      for j = 1:dims
        fprintf(stdout, "   %1.3f   ", acceptanceRate(b+1, j));
      endfor
      fprintf(stdout, "\n");
      for j = 1:dims
        fprintf(stdout, "+/-%1.3f   ", tuning(j));
      endfor
      fprintf(stdout, "\n\n");
      fflush(stdout);
    endif
  endfor
endfunction


function berger = bergerPrior(sigma)
  berger = sqrt(sigma).^3;
endfunction


## -------------------------------------------------
## From "A nonstationary covariance based kriging method for
## metamodeling in engineering design" by Xiong et. al.
## -------------------------------------------------
function chain = mcmc_nonst_mh(scale, ub, x0_eta, X, y, n, \
                               prior="Jeffrey", xi, nugget=0, FUN = @(x) 1)
  if (rows(y) != rows(X))
    error("The vector y has to have the same dimension as matrix columns.");
  endif

  if (!strcmp(prior, "Jeffrey") && !strcmp(prior, "Berger"))
    error("The given prior is not supported");
  endif

  F = FUN(X);
  k = rows(xi);
  l = columns(xi);

  ## initialise the x values
  x_old.Eta = x0_eta;

  x.Eta = zeros(k, l);
  x.beta = zeros(columns(F), 1);
  x.sigma = 0.0;

  ## "krig_likelihood(sigma, theta, X, y, beta, f)"
  [q_old, x_old.beta, x_old.sigma] = \
      krig_nonst_likelihood(X, y, F, xi, x_old.Eta, nugget);

  ## if the berger prior is required transform sigma
  if (strcmp(prior, "Berger"))
    x_old.sigma = bergerPrior(x_old.sigma);
  endif

  pi_old = 1 / x_old.sigma;

  ## initialise the chain
  chain.accepted = 0;
  chain.beta = zeros(columns(F), n);
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
    [q_new, x.beta, x.sigma] = krig_nonst_likelihood(X, y, F, xi, x.Eta, nugget);

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
      chain.beta(:,chain.accepted) = x_old.beta;
      chain.sigma(chain.accepted) = x_old.sigma;
      chain.l(chain.accepted) = q_old;

      for a = 1:rows(x_old.Eta)
        for b = 1:columns(x_old.Eta)
          chain.Eta(a,b,chain.accepted) = x_old.Eta(a,b);
        endfor
      endfor
    endif
  endfor

  chain.beta = chain.beta(:,1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.Eta = chain.Eta(:,:,1:chain.accepted);
  chain.l = chain.l(1:chain.accepted);
endfunction
