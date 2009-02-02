function gp_dv = findGradient(alpha, width, v, R_inv, X, nugget = 0)
  n = columns(v);
  gp_dv = zeros(1, n);
  theta = e.^v; ## transform the log hyperparameters

  prior = gp_prior_grad(alpha, width, v);

  for i = 1:n
    gp_dv(i) = prior(i) - 1/2 * \
        trace(R_inv * scf_gaussianm_deriv(X, theta, i, nugget));
  endfor
endfunction

## n = number of input dimensions
function en = mc_app_energy(alpha, width, n, rel_cm, R, sigma_sq)
  lp = gp_log_prior(alpha, width, n, rel_cm);
  en = -lp;

  n_i = rows(R);
  l = - 0.5 * (n_i * log(2 * pi * sigma_sq) + log(det(R)) + n_i); #from PhD JD Martin Eq 2.37
  en += -l; ## use - profile log likelihood

endfunction

## the following variables are used by hmc
##  global Taumax ;
##  global grads ; # number of gradients evaluated
##  global x ;
## FUN returns a column vector
function [chain] = hmc (theta, L, eta, Taumax = 13, X, y, alpha, \
                        width, nugget = 0, FUN = @(x) 1)
  w = log(theta);
  ## hmc ( w , L , eta )
  ## w is a column vector
  ## uses HMC on landscape M(w)
  ##                       for a number of loops L
  ##                       with learning rate eta.
  ##
  ## This HMC program is simplistic in that it uses
  ## the Kinetic energy p*p'. In general (where the lengthscales
  ## in the different dimensions are different) it is better to
  ## use p*K*p, where K is a kinetic energy tensor (mass tensor).
  ##
  ## for the crude langevin method, we add noise of magnitude ~ sqrt(eta)
  ## for true langevin we also compute the energy and whether to accept
  ## for hmc, we loop some random number of times round the leapfrog
  ##
  ##                    the final weights are returned
  ## also the weight history is logged to wl every dT its
  ## using the global variable T

  ## calculate the beta/sigma values
  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor

  chain.beta = zeros(columns(F), L);
  chain.theta = zeros(L, 2);
  chain.sigma = zeros(1, L);
  chain.pot = zeros(1, L);

  chain.accepted = chain.rejects = chain.grads = 0;
  num_inputs = columns(theta);
  epsilon = sqrt( 2 * eta )

  n = rows(y);
  R = scf_gaussianm(X, theta, nugget);
  beta = ((F' * (R\F))\F') * (R\y);
  temp = y - F * beta;
  sigma_sq = 1 / n * temp' * (R\temp);
  R_inv = R^-1;
  ## initialize gradient
  gw = findGradient(alpha, width, w, R_inv, X, nugget);
  chain.grads++;

  ## initialize objective function
  M  = mc_app_energy(alpha, width, num_inputs, w, R, sigma_sq);

  for (l = 1:L)                  ## loop L times
    p = randn ( size(w) ) ; ## initial momentum is Normal(0,1)
    H = dot(p, p) / 2 + M;     ## evaluate H(w,p)

    Tau = round(rand () * Taumax) + Taumax;  ## pick trajectory length
    wnew = w ;
    gwnew = gw ;
    ## difference to R. Neil:
    ## 1. tau steps for wnew only, p steps at start and end
    p = p - epsilon * gwnew / 2 ; ## make half-step in p
    for tau = 1:Tau
      wnew = wnew + epsilon * p;    ## make step in w

      ## find new gradient
      R = scf_gaussianm(X, e.^wnew, nugget);
      R_inv = R^-1;
      gwnew = findGradient(alpha, width, wnew, R_inv, X, nugget);
      chain.grads++;
    endfor

    p = p - epsilon * gwnew / 2 ;  ## make half-step in p

    R = scf_gaussianm(X, e.^wnew, nugget);
    beta = ((F' * (R\F))\F') * (R\y);
    temp = y - F * beta;
    sigma_sq = 1 / n * temp' * (R\temp);

    ## find new objective function
    ## Mnew == potential energy
    Mnew  = mc_app_energy(alpha, width, num_inputs, wnew, R, sigma_sq);
    Hnew = dot(p, p) / 2 + Mnew;  ## evaluate new value of H (energy)

##    if ((wnew(1) > width) || (wnew(2) > width))
##      accept = 0;
##    else
      dH = Hnew - H;
      u = rand();
      if ( dH < 0 )
        accept = 1 ;
      elseif ( u < exp ( - dH ) )
        accept = 1 ;
      else
        accept = 0 ;
      endif
##    endif

    if (accept)
      chain.accepted ++ ;
      gw = gwnew;
      w = wnew ;
      M = Mnew ;

      chain.beta(:, chain.accepted) = beta;
      chain.theta(chain.accepted, :) = e.^w;
      chain.sigma(chain.accepted) = sigma_sq;
      chain.pot(chain.accepted) = Mnew;
    else
      chain.rejects ++;
    endif
  endfor

  chain.beta = chain.beta(:,1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.pot = chain.pot(1:chain.accepted);
  chain.theta = chain.theta(1:chain.accepted,:);
endfunction

function gp_dv = findGradientNonst(alpha, width, v, R_inv, X, xi, nugget = 0)
  [r,c] = size(v);
  gp_dv = zeros(r, c);
  eta = e.^v; ## transform the log hyperparameters
  prior = gp_prior_grad(alpha, width, v);

  for i = 1:r ## knots
    for j = 1:c ## input-dimensions
      gp_dv(i, j) = prior(i, j) - 1/2 * \
          trace(R_inv * scf_nonst_m_deriv(X, eta, xi, j, i, nugget));
    endfor
  endfor
endfunction

## the following variables are used by hmc
##  global Taumax ;
##  global grads ; # number of gradients evaluated
##  global x ;
## FUN returns a column vector
function [chain] = hmc_nonst(eta_est, xi, L, eta, Taumax = 13, X, y, alpha, \
                             width, nugget = 0, FUN = @(x) 1)
  w = log(eta_est);
  ## hmc ( w , L , eta )
  ## w is a column vector
  ## uses HMC on landscape M(w)
  ##                       for a number of loops L
  ##                       with learning rate eta.
  ##
  ## This HMC program is simplistic in that it uses
  ## the Kinetic energy p*p'. In general (where the lengthscales
  ## in the different dimensions are different) it is better to
  ## use p*K*p, where K is a kinetic energy tensor (mass tensor).
  ##
  ## for the crude langevin method, we add noise of magnitude ~ sqrt(eta)
  ## for true langevin we also compute the energy and whether to accept
  ## for hmc, we loop some random number of times round the leapfrog
  ##
  ##                    the final weights are returned
  ## also the weight history is logged to wl every dT its
  ## using the global variable T

  ## calculate the beta/sigma values
  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor

  [r,c] = size(eta_est);

  chain.beta = zeros(columns(F), L);
  chain.Eta = zeros(r, c, L);
  chain.sigma = zeros(1, L);
  chain.pot = zeros(1, L);

  chain.accepted = chain.rejects = chain.grads = 0;
  num_inputs = r*c;
  epsilon = sqrt(2 * eta);

  n = rows(y);
  R = scf_nonst_m(X, xi, eta_est, nugget);
  beta = ((F' * (R\F))\F') * (R\y);
  temp = y - F * beta;
  sigma_sq = 1 / n * temp' * (R\temp);
  R_inv = R^-1;
  ## initialize gradient
  gw = findGradientNonst(alpha, width, w, R_inv, X, xi, nugget);
  chain.grads++;

  ## initialize objective function
  M  = mc_app_energy(alpha, width, num_inputs, w, R, sigma_sq);

  for (l = 1:L)                  ## loop L times
    p = randn ( size(w) ) ; ## initial momentum is Normal(0,1)
    H = dot(p, p) / 2 + M;     ## evaluate H(w,p)

    Tau = round(rand () * Taumax) + Taumax;  ## pick trajectory length
    wnew = w ;
    gwnew = gw ;
    ## difference to R. Neil:
    ## 1. tau steps for wnew only, p steps at start and end
    p = p - epsilon * gwnew / 2 ; ## make half-step in p
    for tau = 1:Tau
      wnew = wnew + epsilon * p;    ## make step in w

      ## find new gradient
      R = scf_nonst_m(X, xi, e.^wnew, nugget);
      R_inv = R^-1;
      gwnew = findGradientNonst(alpha, width, wnew, R_inv, X, xi, nugget);
      chain.grads++;
    endfor

    p = p - epsilon * gwnew / 2 ;  ## make half-step in p

    R = scf_nonst_m(X, xi, e.^wnew, nugget);
    beta = ((F' * (R\F))\F') * (R\y);
    temp = y - F * beta;
    sigma_sq = 1 / n * temp' * (R\temp);

    ## find new objective function
    ## Mnew == potential energy
    Mnew  = mc_app_energy(alpha, width, num_inputs, wnew, R, sigma_sq);
    Hnew = dot(p, p) / 2 + Mnew;  ## evaluate new value of H (energy)

    if ((wnew(1) > width) || (wnew(2) > width))
      accept = 0;
    else
      dH = Hnew - H;
      u = rand();
      if ( dH < 0 )
        accept = 1 ;
      elseif ( u < exp ( - dH ) )
        accept = 1 ;
      else
        accept = 0 ;
      endif
    endif

    if (accept)
      chain.accepted ++ ;
      gw = gwnew;
      w = wnew ;
      M = Mnew ;

      chain.beta(:, chain.accepted) = beta;
      chain.sigma(chain.accepted) = sigma_sq;
      chain.pot(chain.accepted) = Mnew;
      chain.Eta(:, :, chain.accepted) = e.^w;
    else
      chain.rejects ++;
    endif
  endfor

  chain.beta = chain.beta(:,1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.pot = chain.pot(1:chain.accepted);
  chain.Eta = chain.Eta(:,:,1:chain.accepted);
endfunction
