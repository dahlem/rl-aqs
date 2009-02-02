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
## Keywords: Canonical Analysis, Kriging, Metamodel
## Created: 10.11.2008
## Version: 0.1


## boundaries: the boundaries of the input variables. The first row
## represents the minimum and the 2nd row the maximum
## meshsize: number of design points
function D = ca_meshSobol(boundaries, meshsize)
  nvar = columns(boundaries);
  S = sobol(nvar, meshsize)';
  D = zeros(meshsize, nvar);

  for j=1:nvar
    ## scale the random variable to [min, max]
    D(:,j) = boundaries(1,j) + S(:,j) .* (boundaries(2,j) - boundaries(1,j));
  end
endfunction


function D = ca_mesh(boundaries, meshsize)
  stepsize1 = (boundaries(2, 1) - boundaries(1, 1)) / meshsize;
  stepsize2 = (boundaries(2, 2) - boundaries(1, 2)) / meshsize;

  [xx, yy] = meshgrid(boundaries(1, 1):stepsize1:boundaries(2, 1),
                      boundaries(1, 2):stepsize2:boundaries(2, 2));
  D = [vec(xx), vec(yy)];
endfunction


function X = ca_modelM(D)
  n = rows(D);
  nvar = columns(D);
  cross_prod = factorial(nvar)/2;
  dim = 1 + cross_prod + 2 * nvar;
  X = ones(n, dim);
  cols = 1 + nvar;

  for i = 1:nvar
    X(:, 1 + i) = D(:, i);
  endfor

  for i = 1:(nvar-1)
    for j = (i+1):nvar
      cols++;
      X(:, cols) = D(:, i) .* D(:, j);
    endfor
  endfor

  for i = 1:nvar
    X(:, 1 + cross_prod + nvar + i) = D(:, i).^2;
  endfor
endfunction


function b = ca_lse(X, y)
  b = (X' * X)^-1 * X' * y;
endfunction


function w = ca_w(x, xS)
  w = x - xS;
  w = w';
endfunction


function B = ca_coeffM(b, nvar)
  cross_prod = factorial(nvar)/2;
  B = zeros(nvar, nvar);

  for i = 1:nvar
    for j = i:nvar
      if (i == j)
        B(i,i) = b(1+nvar+cross_prod+i);
      else
        B(i,j) = b(1+nvar+i)/2;
        B(j,i) = B(i,j);
      endif
    endfor
  endfor
endfunction


function y = ca_coord1(yS, xS, x, B)
  w = ca_w(x, xS);
  y = yS + w^T * B * w;
endfunction


function m = ca_eigNorm(evec)
  m = 1/norm(evec) * evec;
endfunction


function M = ca_eigNormM(B)
  [v, lambda] = eig(B);
  dim = rows(v);
  M = zeros(dim, dim);

  for i = 1:dim
    M(:,i) = ca_eigNorm(v(:,i));
  endfor
endfunction


## Stationary point within the design domain
function [y,z,w] = ca_modelW(yS, xS, M, B, x)
  w = ca_w(x, xS);
  z = M' * w;
  L = M' * B * M;
  y = yS + z' * L * z;
endfunction


function [y,z,w] = ca_modelWM(yS, xS, M, D, B)
  n = rows(D);
  nvar = columns(D);
  y = zeros(n, 1);
  w = zeros(nvar, n);
  z = zeros(nvar, n);

  L = M' * B * M;

  for i = 1:n
    w(:,i) = ca_w(D(i,:), xS);
    z(:,i) = M' * w(:,i);
    y(i) = yS + z(:,i)' * L * z(:,i);
  endfor
endfunction


## Stationary point not within the design domain
function [y, z, theta] = ca_modelX(M, B, b, x)
  nvar = columns(M);
  b0 = b(1);
  b_lin = b(2:(1+nvar));

  L = M' * B * M;
  theta = M' * b_lin;

  z = M' * x;
  y = b0 + z' * theta + z' * L * z;
endfunction


function [y, z, theta] = ca_modelXM(M, D, B, b)
  n = rows(D);
  nvar = columns(D);
  y = zeros(n, 1);
  z = zeros(nvar, n);
  b0 = b(1);
  b_lin = b(2:(1+nvar));

  L = M' * B * M;
  theta = M' * b_lin;

  for i = 1:n
    z(:,i) = M' * D(i,:)';
    y(i) = b0 + z(:,i)' * theta + z(:,i)' * L * z(:,i);
  endfor
endfunction


function [ym,z,w,B,D,M,lambda,thetam] = ca_analyse(meshsize, boundaries, yS, xS, \
                                                   S, R, beta, theta, \
                                                   y, F, FUN)
  D = ca_mesh(boundaries, meshsize);
  X = ca_modelM(D);

  x1_vec = X(:,1)';
  x2_vec = X(:,2)';
  yk = zeros(rows(X), 1);

  for i = 1:columns(x1_vec)
    yk(i) = krig([x1_vec(i), x2_vec(i)], S, R, beta, theta, y, F, FUN);
  endfor

  b = ca_lse(X, yk);
  B = ca_coeffM(b, columns(D));
  M = ca_eigNormM(B);

  ## eigenvalues
  lambda = diag(M' * B * M);
  thetam = zeros(size(lambda));
  thetam(:,:) = Inf;
  ym = z = w = [];

  ## decided whether to perform minimum/maximum or saddle point analysis
  if ((lambda < 0) || (lambda > 0))
    ## perform minimum/maximum point analysis
    [ym,z,w] = ca_modelWM(yS, xS, M, D, B);
  else
    ## perform saddle point analysis
    [ym,z,thetam] = ca_modelXM(M, D, B, b);
  endif
endfunction


function ca_serialiseModel(dir, prefix, D, z, y)
  fd = fopen([dir, "/", prefix, "-ca-model.dat"], "wt");

  for i = 1:columns(D)
    var = sprintf("x%d,", i);
    fprintf(fd, "%s", var);
  endfor
  for i = 1:rows(z)
    var = sprintf("z%d,", i);
    fprintf(fd, "%s", var);
  endfor
  fprintf(fd, "y\n");

  for i = 1:rows(D)
    var = "";
    for j = 1:columns(D)
      var = sprintf("%s%.10f,", var, D(i, j));
    endfor
    for j = 1:rows(z)
      var = sprintf("%s%.10f,", var, z(j, i));
    endfor
    fprintf(fd, "%s%.10f\n", var, y(i));
  endfor

  fclose(fd);
endfunction


function ca_serialiseStatP(dir, prefix, xS, yS)
  fd = fopen([dir, "/", prefix, "-ca-statP.dat"], "wt");

  for i = 1:columns(xS)
    var = sprintf("x%d,", i);
    fprintf(fd, "%s", var);
  endfor
  fprintf(fd, "y\n");

  for i = 1:columns(xS)
    var = "";
    fprintf(fd, "%.10f,", xS(i));
  endfor
  fprintf(fd, "%.10f\n", yS)

  fclose(fd);
endfunction


function ca_serialiseEigen(dir, prefix, lambda, theta, M)
  fd = fopen([dir, "/", prefix, "-ca-eigen.dat"], "wt");

  fprintf(fd, "lambda,");
  fprintf(fd, "theta,");
  for i = 1:columns(M)
    if (i == columns(M))
      var = sprintf("M%d\n", i);
      fprintf(fd, "%s", var);
    else
      var = sprintf("M%d,", i);
      fprintf(fd, "%s", var);
    endif
  endfor

  for i = 1:rows(lambda)
    fprintf(fd, "%e,", lambda(i));
    if (isfinite(theta(i)))
      fprintf(fd, "%e,", theta(i));
    else
      fprintf(fd, "NaN,");
    endif
    for j = 1:columns(M)
      if (j == columns(M))
        fprintf(fd, "%e\n", M(i,j));
      else
        fprintf(fd, "%e,", M(i,j));
      endif
    endfor
  endfor

  fclose(fd);
endfunction
