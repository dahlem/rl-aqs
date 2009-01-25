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
## Keywords: Latin Hypercube Sampling
## Created: 29.01.2008
## Version: 0.1

## S = lhs(n, min, max)
## uniform Latin Hypercube Sampling (from matlab)
## Input:
##  n   : number of samples
##  min : vector (x_1, x_2, ..., x_k) of the minimum values for each dimension
##  max : vector (x_1, x_2, ..., x_k) of the maximum values for each dimension
function S = deslhs(n, min, max)
  if ((nargin ~= 3) || ~isnumeric(n) || (length(min) != length(max)))
    error("usage: lhs(n, min, max)");
    error(", where n is the number of samples");
    error("min is the vector of minimum values for each dimension");
    error("max is the vector of maximum values for each dimension.");
  endif

  nvar = length(min);
  u = rand(n, nvar);
  S = zeros(n, nvar);

  for (j=1:nvar)
    ## generate the random permutation of the integers 1...n
    idx = randperm(n);

    P = (idx' - u(:,j)) / n;

    ## scale the random variable to [min, max]
    S(:,j) = min(j) + P .* (max(j) - min(j));
  endfor
endfunction
