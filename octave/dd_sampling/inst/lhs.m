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


## from Ye: Orthogonal Latin Hypercube
function M = deslhsoM(m, e)
  I = eye(2);
  R = [0,1;1,0];

  M = [e];
  A = zeros(2^(m-1), 2^(m-1), m-2);

  for k=1:(m-1)
    temp = 1;

    ## multiply the Is
    for i=1:(m-1-k)
      if (i == 1)
        temp = I;
      else
        temp = kron(temp, I);
      endif
    endfor

    ## multiply the Rs
    for i=1:k
      temp = kron(temp, R);
    endfor

    A(:, :, k) = temp;
    M = [M, temp * e];
  endfor

  for j=1:(m-2)
    M = [M, A(:, :, m-1) * A(:, :, j) * e];
  endfor
endfunction


function S = deslhsoS(m)
  Bk = [-1; 1];
  Bi = [1; 1];

  S = [ones(2^(m-1), 1)];

  for k=1:(m-1)
    temp = 1;

    ## multiply the Is
    for i=1:(m-1)
      if (i != (m - k))
        temp = kron(temp, Bi);
      else
        temp = kron(temp, Bk);
      endif
    endfor

    S = [S, temp];
  endfor

  for j=1:(m-2)
    S = [S, S(:, 2) .* S(:, j + 2)];
  endfor
endfunction


## construct a LH design with 2m - 2 input variables and 2^m + 1 locations
function T = deslhso(m, min, max)
  e = (1:(2^(m-1)))'; ## we consider the top half here

  M = deslhsoM(m, e);
  S = deslhsoS(m);
  T = M .* S;
  Tmirror = T * -1;

  ## assemble the T matrix with a centre point and the mirror image of T
  T = [T; zeros(2 * m - 2, 1)'; Tmirror];

  ## move the design matrix into having positive values
  T = T .+ (2^m)/2;

  ## normalise the matrix to be unit length
  T = T ./ 2^m;

  ## scale the matrix to be within [min, max]
  T = repmat(min, rows(T), 1) .+ T .* repmat(max - min, rows(T), 1);
endfunction


function T = deslhsoMaximin(m, r, lb, ub)
  ## 1. randomly select e
  e = randperm(2^(m-1))';

  ## 1.1 Construct LHS
  M = deslhsoM(m, e);
  Mt = M;

  ## 1.2 Evaluate minimum distance
  d = euclideanDistance(M);
  cr = min(d);

  for n = 1:r
    ## 2. Generate [2^(m-1) * (2^(m-1) + 1)] permutations of e by
    ##    transposing i and j
    for i = 1:(2^(m-1) * (2^(m-1) + 1))
      e1 = e;
      [r1, r2] = uniqueInts(2^(m-1));
      e1(r1) = e(r2);
      e1(r2) = e(r1);

      ## 2.1 For each e, construct LHS and evaluate minimum distance
      M = deslhsoM(m, e1);
      d = euclideanDistance(M);
      crNew = min(d);

      ## 2.2 Remember the best LHS design
      if (crNew > cr)
        cr = crNew;
        Mt = M;
      endif
    endfor

    ## 3. Repeat step 2 with r different e
    e = randperm(2^(m-1))';
  endfor

  S = deslhsoS(m);
  T = Mt .* S;
  Tmirror = T * -1;

  ## assemble the T matrix with a centre point and the mirror image of T
  T = [T; zeros(2 * m - 2, 1)'; Tmirror];

  ## move the design matrix into having positive values
  T = T .+ 2^(m-1);

  ## normalise the matrix to be unit length
  T = T ./ 2^m;

  ## scale the matrix to be within [min, max]
  T = repmat(lb, rows(T), 1) .+ T .* repmat(ub - lb, rows(T), 1);
endfunction


function d = euclideanDistance(M)
  [r,c] = size(M);
  d = [];

  for i=1:(c-1)
    for j=(i+1):c
      diff = M(:,i) - M(:,j);
      d = [d; sqrt(dot(diff, diff))];
    endfor
  endfor
endfunction


function [r1, r2] = uniqueInts(maxVal)
  r1 = unidrnd(maxVal);

  do
    r2 = unidrnd(maxVal);
  until (r1 != r2)
endfunction
