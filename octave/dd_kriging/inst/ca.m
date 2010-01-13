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

function D = gen3fact(n, boundaries)
  dim = 3^n;
  D = zeros(dim, n);
  for i = 1:dim
    j = i - 1;
    for l = 1:n
      D(i, l) = 1 + mod(j, 3);
      j = floor(j / 3);
    endfor
  endfor

  ## scale to be between [0;1]
  D = D - 1;
  D = D./2;
  D = repmat(boundaries(1,:), rows(D), 1) .+ D .* repmat(boundaries(2,:) - boundaries(1,:), rows(D), 1);
endfunction

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
  stepsize3 = (boundaries(2, 3) - boundaries(1, 3)) / meshsize;

  [xx, yy, zz] = meshgrid(boundaries(1, 1):stepsize1:boundaries(2, 1),
                          boundaries(1, 2):stepsize2:boundaries(2, 2),
                          boundaries(1, 3):stepsize3:boundaries(2, 3));
##  [xx, yy] = meshgrid(boundaries(1, 1):stepsize1:boundaries(2, 1),
##                      boundaries(1, 2):stepsize2:boundaries(2, 2));

  D = [vec(xx), vec(yy), vec(zz)];
##  D = [vec(xx), vec(yy)];
endfunction


function D = ca_mesh2D(boundaries, meshsize)
  stepsize1 = (boundaries(2, 1) - boundaries(1, 1)) / meshsize;
  stepsize2 = (boundaries(2, 2) - boundaries(1, 2)) / meshsize;

  x = boundaries(1, 1):stepsize1:boundaries(2, 1);
  y = boundaries(1, 2):stepsize2:boundaries(2, 2);

  [xx, yy] = meshgrid(x, y);

  D = [vec(xx), vec(yy)];
endfunction


function X = ca_modelM(D)
  n = rows(D);
  nvar = columns(D);
  X = ones(n, 1);

  for i = 1:nvar
    X = [X, D(:, i)];
  endfor

  squaresStart = 1 + nvar;

  for i = 1:nvar
    X = [X, D(:, i).^2];
  endfor

  squaresEnd = squaresStart + nvar;
  for i = 1:(nvar-1)
    for j = (i+1):nvar
      X = [X, D(:, i) .* D(:, j)];
    endfor
  endfor
endfunction


function b = ca_lse(X, y)
  b = (X' * X)^-1 * X' * y;
endfunction


function w = ca_w(x, xS)
  w = x - xS;
  w = w';
endfunction


function xs = ca_statX(B, b)
  xs = -0.5 * B^-1 * b
endfunction


function B = ca_coeffM(b, nvar)
  B = zeros(nvar, nvar);

  for i = 1:nvar
    for j = i:nvar
      if (i == j)
        B(i,i) = b(1 + nvar + i);
      else
        B(i,j) = b(1 + 2 * nvar + i)/2;
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
  w = ca_w(x, xS); ## w equivalent to z
  z = M' * w; ## M equivalent to P in "response surface methodology" by
              ## Myers et.a.
  L = M' * B * M; ## L equivalent to inverse V
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


function [X_r, y_r] = ca_reducedModel(X, yk, y_upper)
  X_r = X;
  y_r = yk;

  for i = 1:n
    if (y_r(i) > yk)
      X_r(i,:) = [];
      y_r(i) = [];
    endif
  endfor
endfunction


function [y] = ca_sndOrderModel(x, b0, b, B)
  y = b0 + x' * b + x' * B * x;
endfunction


function [y] = ca_sndOrderModel1(x, b, B)
  y = -(b(1) + x' * b(2:3) + x' * B * x);
endfunction


function [ym,z,w,b,B,D,M,lambda,thetam] = ca_analyse(boundaries, yS, xS, S, R, C, sigma, theta, y, F, FUN)
  D = gen3fact(columns(boundaries));

  for i = 1:columns(boundaries)
    D(:,i) = boundaries(1,i) + ((D(:,i) - 1) / 2) * (boundaries(2,i) - boundaries(1,i));
  endfor
  X = ca_modelM(D);

  yk = zeros(rows(X), 1);

  for i = 1:rows(D)
    yk(i) = krigS(D(i,:), S, R, C, sigma, theta, y, F, FUN, p);
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


function X = backscale_design(XScaled, lb, ub)
  X = repmat(lb', rows(XScaled),1) + XScaled .* (repmat(ub', rows(XScaled),1) - repmat(lb', rows(XScaled),1));
endfunction


function XScaled = scale_design(X, lb, ub)
  XScaled = (X - repmat(lb', rows(X),1)) ./ (repmat(ub', rows(X),1) - repmat(lb', rows(X),1));
endfunction


function ca_serialiseRidge(prefix, B, b0, b, f, lambdas, increment, start=-1000000, dist, thresh=0.01, caminx, camaxx, minX, maxX, scale=1)
  outfile = [prefix "-ridge_path-" int2str(scale) ".dat"];
  fd = fopen(outfile, "wt");
  ## calculate the ridge path
  fprintf(stdout, "analyse minimum path\n");
  fflush(stdout);
  [xP, RP, yP, incr] = ca_analyseRidgeFocalIntermediate(B, b0, b, f, start:increment:floor(lambdas(1) - increment), dist);

  ## serialise the ridge path
  for r = 1:rows(incr)
    if (scale == 1)
      des = backscale_design(xP(r,:), caminx', camaxx');
    elseif (scale == 2)
      des = scale_design(backscale_design(xP(r,:), caminx', camaxx'), minX', maxX');
    else
      des = xP(r,:);
    endif


    fprintf(fd, "%.10f,%.10f,%.10f,", incr(r), RP(r), yP(r));
    for j = 1:(rows(caminx')-1)
      fprintf(fd, "%.10f,", des(j));
    endfor
    fprintf(fd, "%.10f,0\n", des(rows(caminx')));
  endfor

  for i = 1:(rows(lambdas) - 1)
    ## calculate the ridge path
    fprintf(stdout, "analyse %d centre paths\n", i);
    fflush(stdout);
    [xP, RP, yP, incr] = ca_analyseRidgeFocalIntermediate(B, b0, b, f, floor(lambdas(i) + increment):increment:floor(lambdas(i+1) - increment), dist);

    ## serialise the ridge path
    for r = 1:rows(incr)
      if (scale == 1)
        des = backscale_design(xP(r,:), caminx', camaxx');
      elseif (scale == 2)
        des = scale_design(backscale_design(xP(r,:), caminx', camaxx'), minX', maxX');
      else
        des = xP(r,:);
      endif

      fprintf(fd, "%.10f,%.10f,%.10f,", incr(r), RP(r), yP(r));
      for j = 1:(rows(caminx')-1)
        fprintf(fd, "%.10f,", des(j));
      endfor
      fprintf(fd, "%.10f,%d\n", des(rows(caminx')), i);
    endfor
  endfor

  ## calculate the 2q ridge path
  fprintf(stdout, "analyse maximum paths\n");
  fflush(stdout);
  [xP, RP, yP, incr] = ca_analyseRidgeFocal(B, b0, b, f, lambdas(rows(lambdas)), increment, dist, thresh);

  ## serialise the 2q ridge path
  for r = 1:rows(incr)
    if (scale == 1)
      des = backscale_design(xP(r,:), caminx', camaxx');
    elseif (scale == 2)
      des = scale_design(backscale_design(xP(r,:), caminx', camaxx'), minX', maxX');
    else
      des = xP(r,:);
    endif


    fprintf(fd, "%.10f,%.10f,%.10f,", incr(r), RP(r), yP(r));
    for j = 1:(rows(caminx')-1)
      fprintf(fd, "%.10f,", des(j));
    endfor
    fprintf(fd, "%.10f,%d\n", des(rows(caminx')), rows(lambdas));
  endfor
  fclose(fd);
endfunction


## path is specified by the eigenvalue and the increment
function [xP, RP, yP, incr] = ca_analyseRidgeFocalIntermediate(B, b0, b, f, increments, dist)
  R = +Inf;
  xP = RP = yP = incr = [];

  for i = increments
    ## 1. choose a value of my according to the path
    mu = i;

    ## 2. obtain x and R
    [x, R] = ca_ridgeAnalysisWithFocal(B, b, f, mu);

    if (R <= dist)
      ## 3. evaluate the model y
      yh = ca_sndOrderModel(x, b0, b, B);

      ## 4. tabulate x, R, model y
      xP = [xP; x'];
      RP = [RP; R];
      yP = [yP; yh];
      incr = [incr; mu];
    endif
  endfor
endfunction


## path is specified by the eigenvalue and the increment
function [xP, RP, yP, incr] = ca_analyseRidgeFocal(B, b0, b, f, lambda, increment, dist, thresh=0.01)
  R = +Inf;
  mu = lambda;
  xP = RP = yP = incr = [];

  while (R > thresh)
    ## 1. choose a value of my according to the path
    mu = mu + increment;

    ## 2. obtain x and R
    [x, R] = ca_ridgeAnalysisWithFocal(B, b, f, mu);

    if (R <= dist)
      ## 3. evaluate the model y
      yh = ca_sndOrderModel(x, b0, b, B);

      ## 4. tabulate x, R, model y
      xP = [xP; x'];
      RP = [RP; R];
      yP = [yP; yh];
      incr = [incr; mu];
    endif
  endwhile
endfunction


## b must only contain the linear terms, i.e., b_1, b_2, ..., b_n
function [x, R] = ca_ridgeAnalysis(B, b, mu)
  x = -0.5 * (B - mu * eye(rows(B)))^-1 * b;
  R = sqrt(x' * x);
endfunction


function [x, R] = ca_ridgeAnalysisWithFocal(B, b, f, mu)
  x = -0.5 * (B - mu * eye(rows(B)))^-1 * (b + 2 * mu * f);
  R = sqrt((x - f)' * (x - f));
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
