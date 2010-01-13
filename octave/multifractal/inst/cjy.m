## Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: multifractal random walk
## Created: 12.01.2010
## Version: 0.1

## d0 = 1, a0 = 0.1, b = 1.3, lambda = 1.05, Nc = 1000, T = 5000, n0 = 0, nmax = log(30)/log(lambda)
function [rt, time] = cjy(d0, a0, b, lambda, Nc, T, n0, nmax)
  ## eq. 5
  a = a0/log(lambda);

  ## eq. 6
  p = 1/(1 + lambda^2);

  etaB = 2 * p - 1;
  nt = n0;
  ## generate etat random variables assuming values +/-1
  eta = discrete_rnd(T, [+1; -1], [p; 1-p]);
  
  ## generate Ki
  Ki = 1./sqrt([1:(Nc+1)]');
  
  ## start iteration for t
  ## eq. 4
  n = Nc;
  rt = zeros(1, T);
  time = zeros(1, T);
  
  for (t = 1:T)
    temp1 = Ki(Nc + 1);
    if ((t - Nc - 1) > 0)
      temp1 = temp1 * eta(t-Nc-1);
    else
      n = t - 1;
      temp1 = 0;
    endif

    temp2 = 0;
    if (t > 1)
      temp2 = (Ki(2:(n+1)) - Ki(1:n)) .* eta((t-1):-1:(t-n));
    endif

    phit = Ki(1) * eta(t) - temp1 + sum(temp2);

    ## eq. 3
    dnt = eta(t) + a * phit - b * etaB;
    nt = nt + dnt;

    ## bound the nt to avoid too large fluctuations
    if (nt < 0)
      nt = 0;
    elseif (nt > nmax)
      nt = nmax;
    endif
    
    ## eq. 2
    dt = d0 * lambda^nt;
    ## eq. 1
    rt(t) = dt * normrnd(0, 1);
    time(t) = t;
  endfor
endfunction
