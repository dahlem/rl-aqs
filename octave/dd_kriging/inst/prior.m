function p = prior_pick(width, alpha, n)
  p = log(prior_pick_sigma(prior_width_scaled(width, alpha, n), alpha));
endfunction


function g = prior_pick_sigma(sigma, alpha)
  p = 0;
  
  if (alpha == 0)
    p = 1;
  else
    p = gamma_rnd(alpha/2, 1) / (alpha/2);
  endif

  g = sigma/sqrt(p);
endfunction


## width
## alpha
## n: number of inputs
function p = prior_width_scaled(width, alpha, n)
  scale = 0;

  if (alpha == 0) ## Infinite
    scale = n;
  elseif (alpha > 2)
    scale = n * (alpha/(alpha-2));
  elseif (alpha == 2)
    if (n < 3)
      scale = n;
    else
      scale = n * log(n);
    endif
  else
    scale = n^(2/alpha);
  endif

  p = width / sqrt(scale);
endfunction


## rel_cm is the log of the hyperparameter
## n is the number of input dimensions
function lp = gp_log_prior(alpha, width, n, rel_cm)
  lp = 0;
  width_scaled = prior_width_scaled(width, alpha, n);
  
  for i = 1:columns(rel_cm)
    lp += gp_gdens(alpha, width_scaled, rel_cm(i));
  endfor
endfunction


## FIND LOG DENSITY FOR HYPERPARAMETER.  Computes the log of the density for 
## the log of a hyperparameter which in precision form has a gamma density with
## shape and scale parameter as given.  Constant factors (depending only on
## the shape parameter) may be omitted if the last parameter is zero. 
## The log density for a gamma variable, x, is given by
##     (alpha/2 - 1) * log(x) - x * alpha/(2*mean) 
##        + (alpha/2) * log(alpha/(2*mean)) - lgamma(alpha/2) 
## where lgamma is the log of the gamma function.  The last terms depends
## only on alpha, and so can be omitted if the last parameter is zero.
## It is in their "precision" form that the hyperparameters have gamma 
## prior densities, as above.  The value actually represented is the log 
## of the "width" form, that is v = log(1/sqrt(x)) = -(1/2)*log(x).  The
## log density for v is therefore the log density for x shown above plus
## log(x) + log(2).  The mean precision in terms of the 'width' in the 
## prior specification is 1/(width*width).
function lp = gp_gdens(alpha, width, v)
  mean = 1/(width*width);
  lp = (alpha/2) * (-2*v) - exp(-2*v) * alpha/(2*mean) + (alpha/2) * log(alpha/(2*mean));
endfunction


## FIND GRADIENT OF MINUS LOG PRIOR DENSITY.  The derivatives of minus the
## log density of the hyperparameters (in logarithmetic form) are found,
## and stored in the last parameter. 
## The derivatives are found by first setting them all to zero, and then
## looking at each contribution to the prior density (corresponding to one
## conditional probability of gamma form), and adding to the derivatives
## of the (up to) two hyperparameters that it may involve.
## v == the log of the hyperparameters
function gp_dv = gp_prior_grad(alpha, width, v)
##function [gp_dwidth, gp_dv] = gp_prior_grad(alpha, width, v, dwidth, dv)
  gp_dv = gp_gdiff(alpha, width, v);
endfunction


## FIND DERIVATIVES OF MINUS LOG DENSITY. Finds the derivatives of minus
## the log prior density for a hyperparameter in logarithmic form with respect 
## to the log of the width parameter and the hyperparameter itself (in log
## form). These derivatives are added to the values to which pointers are
## passed. A zero pointer suppresses calculation of the corresponding 
## derivative.
function gp_dv = gp_gdiff(alpha, width, v)
##function [gp_dwidth, gp_dv] = gp_gdiff(alpha, width, v, dwidth, dv)
  mean = 1/(width*width);
##  gp_dwidth = dwidth + exp(-2*v)*alpha/mean - alpha;
  gp_dv = alpha - exp(-2*v)*alpha/mean;
endfunction
