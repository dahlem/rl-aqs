function gp_dv = findGradient(alpha, width, v, R_inv, X, nugget = 0)
  n = columns(v);
  gp_dv = zeros(1, n);
  theta = e.^v; ## transform the log hyperparameters

  for i = 1:n
    gp_dv = gp_prior_grad(alpha, width, v) - 1/2 * \
        trace(R_inv * scf_gaussianm_deriv(X, theta, i, nugget));
  endfor
endfunction

## n = number of input dimensions
function en = mc_app_energy(alpha, width, n, rel_cm, R, sigma_sq)
  lp = gp_log_prior(alpha, width, n, rel_cm);
  en = -lp;

  n_i = rows(R);
  l = - 0.5 * (n_i * log(2 * pi * sigma_sq) + log(det(R)) + n_i); #from PhD JD Martin Eq 2.37
  en += l;

endfunction

## the following variables are used by hmc
##  global Taumax ;
##  global grads ; # number of gradients evaluated
##  global x ;
## what is the width??
function [chain] = hmc (theta, L, eta, Taumax = 13, X, y, alpha, width, nugget = 0)
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

  chain.beta = zeros(1, L);
  chain.theta = zeros(L, 2);
  chain.sigma = zeros(1, L);
  chain.pot = zeros(1, L);

  chain.accepted = chain.rejects = chain.grads = 0;
  num_inputs = columns(theta);
  epsilon = sqrt( 2 * eta )

  ## calculate the beta/sigma values
  f = ones(rows(X), 1);
  n = rows(y);
  R = scf_gaussianm(X, theta, nugget);
  R_inv = inv(R);

  beta = (f' * R_inv * f)^-1 * f' * (R_inv * y);
  temp = y - f * beta;
  sigma_sq = 1 / n * temp' * R_inv * temp;
  
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
      R_inv = inv(R);
      gwnew = findGradient(alpha, width, wnew, R_inv, X, nugget);
      chain.grads++;
    endfor

    p = p - epsilon * gwnew / 2 ;  ## make half-step in p

    R = scf_gaussianm(X, e.^wnew, nugget);
    beta = (f' * (R\f))^-1 * f' * (R\y);
    temp = y - f * beta;
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

      chain.beta(chain.accepted) = beta;
      chain.theta(chain.accepted, :) = e.^w;
      chain.sigma(chain.accepted) = sigma_sq;
      chain.pot(chain.accepted) = Mnew;
    else
      chain.rejects ++;
    endif
  endfor

  chain.beta = chain.beta(1:chain.accepted);
  chain.sigma = chain.sigma(1:chain.accepted);
  chain.pot = chain.pot(1:chain.accepted);
  chain.theta = chain.theta(1:chain.accepted,:);
endfunction
