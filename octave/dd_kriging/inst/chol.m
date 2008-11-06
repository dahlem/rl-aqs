## Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: linear system, Cholesky decomposition, Cholesky-Crout reduction
## Created: 11.12.2007
## Version: 1.0


1;

function L = dd_cholcrout(A)
  if (nargin != 1)
    usage("dd_cholcrout(A)");
  endif

  if (matrix_type(A) != "Positive Definite")
    error("The matrix A has to be positive definite.");
  endif

  L = zeros(rows(A), columns(A));

  for j = 1:columns(A)
    for i = j:rows(A)
      temp = 0;
      
      if (i > j)
	for k = 1 : (j - 1)
	  temp += L(i, k) * L(j, k);
	endfor
	
	L(i, j) = (A(i, j) - temp) / L(j, j);
      elseif (i == j)
	for k = 1 : (i - 1)
	  temp += L(i, k)^2;
	endfor
	
	L(i, i) = sqrt(A(i, i) - temp);
      endif
    endfor
  endfor
endfunction


function [x_bar, x_error, max_error] = dd_chol(A, b, x)
  if (nargin != 3)
    usage("dd_chol(A, b, x)");
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

  L = dd_cholcrout(A);
  
  ML = [L, b];
  [ML, y_bar] = dd_forwards(ML);

  MU = [L', y_bar];
  [MU, x_bar] = dd_backwards(MU);
  
  x_error = x - x_bar;
  max_error = max(abs(x_error));
  
endfunction

#!test
#! A = [1.86279, 0.47863, -0.54877; 0.47863, 1.61609, 0.10628; -0.54877,
#!      0.10628, 2.76115]
#! L = dd_cholcrout(A)
#! assert(L * L', A, 0.001)
#! b = [1;2;3]
#! x = [1;1;1]
#! [x_bar, x_error, max_error] = dd_chol(A, b, x)
#! assert(A * x_bar, b, 0.001)
