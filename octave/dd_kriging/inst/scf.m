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


## This function implements the gaussian spatial correlation between
## two vectors and a correlation vector theta. The correlation vector
## theta correlates each dimension of the two vectors.
function r = scf_gaussian(x, y, theta)
  if (nargin != 3)
    usage("scf_gaussianv(x, y, theta)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  if (columns(x) != columns(y))
    error("The vectors x and y have to have the same dimension.");
  endif

  r = 1.0;
  
  for i = 1:columns(x)
    r *= e^-((y(i) - x(i))^2 / theta(i));
  endfor
endfunction


function R = scf_gaussianm(X, theta)
  if (nargin != 2)
    usage("scf_gaussianm(X, theta)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  R = eye(rows(X));

  for i = 1:rows(R)
    for j = i+1:columns(R)
      R(i, j) = scf_gaussian(X(i,:), X(j,:), theta);
      R(j, i) = R(i, j);
    endfor
  endfor
endfunction


function r = scf_gaussianu(X, x, theta)
  if (nargin != 3)
    usage("scf_gaussianu(X, x, theta)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  if (columns(x) != columns(X))
    error("The vector x has to have the same dimension as matrix columns.");
  endif

  r = zeros(rows(X), 1);

  for i = 1:rows(X)
    r(i,:) = scf_gaussian(x, X(i,:), theta);
  endfor
endfunction
