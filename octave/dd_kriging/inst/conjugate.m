## Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
##  
## This file is free software; as a special exception the author gives
## unlimited permission to copy and/or distribute it, with or without 
## modifications, as long as this notice is preserved.
## 
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
## implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## -*- texinfo -*-
## @deftypefn{Function File} {[@var{x_bar}, @var{x_error}, @var{max_error}] = }
##     dd_conjugate (@var{A}, @var{b}, @var{x})
## @cindex Conjugate Gradient, Iterative Method
##
## @var{A} is the matrix to be solved with the solution vector @var{b}
## and a given x vector @var{x}.
## @var{x_bar} carries the solution.
##
## @end deftypefn


function [x_bar, x_error, max_error, k] = dd_conjugate(A, b, x, thresh=e^-12)
  if (nargin != 4)
    usage("dd_conjugate(A, b, x, threshold)");
  endif

  if (matrix_type(A) != "Positive Definite")
    error("The matrix A has to be positive definite.");
  endif

  if (rows(b) != rows(A))
    error("The vector b has to have the same dimension as rows in matrix A");
  endif

  if (!isvector(b))
    error("b must be an array.");
  endif

  x_bar = x;
  p = b - A * x_bar;
  r = p;
  alpha = 0;
  beta = 0;
  k = 1;
  error = dot(r, r);

  do
    temp = A * p;
    alpha = error / (dot(p, temp));
    x_bar = x_bar + (alpha * p);
    r = r - alpha * temp;
    dotprod = dot(r, r);
    beta = dotprod / error;
    p = r + beta * p;
    error = dotprod;
    k++;
  until ((k > rows(A)) || (error < thresh))  
  
  x_error = x - x_bar;
  max_error = max(abs(x_error));
  
endfunction

#!test
#! A = [1.86279, 0.47863, -0.54877; 0.47863, 1.61609, 0.10628; -0.54877,
#!      0.10628, 2.76115]
#! b = [1;2;3]
#! x = [0;0;0]
#! [x_bar, x_error, max_error] = dd_conjugate(A, b, x)
#! assert(A * x_bar, b, 0.001)
