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
## Keywords: spatial correlation functions, gaussian, product
## correlation rule
## Created: 20.01.2008
## Version: 0.1


## This function implements the gaussian spatial correlation between
## two vectors and a correlation vector theta. The correlation vector
## theta correlates each dimension of the two vectors.
function r = scf_gaussian(x, y, theta)
  if (nargin < 3)
    usage("scf_gaussianv(x, y, theta, nugget)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  if (columns(x) != columns(y))
    error("The vectors x and y have to have the same dimension.");
  endif

  temp = sum(-(((y - x).^2) .* theta));
  r = e^temp;
endfunction


function R = scf_gaussianm(X, theta, nugget = 0)
  if (nargin != 3)
    usage("scf_gaussianm(X, theta, nugget)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  r = ones(rows(X), 1);
  r = r + nugget;
  R = diag(r);

  for i = 1:rows(R)
    for j = i+1:columns(R)
      R(i, j) = scf_gaussian(X(i,:), X(j,:), theta, nugget);
      R(j, i) = R(i, j);
    endfor
  endfor
endfunction


function Rd = scf_gaussianm_deriv(X, theta, deriv, nugget = 0)
  if (nargin != 4)
    usage("scf_gaussianm_deriv(X, theta, deriv, nugget)");
  endif

  if (theta <= 0)
    error("The parameter theta has to be positive");
  endif

  Rd = zeros(rows(X));

  for i = 1:rows(Rd)
    for j = i+1:columns(Rd)
      Rd(i, j) = - scf_gaussian(X(i,deriv), X(j,deriv), theta(deriv), nugget) * \
          (X(i,deriv) - X(j,deriv))^2;
      Rd(j, i) = Rd(i, j);
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



## -------------------------------------------------
## From "A nonstationary covariance based kriging method for
## metamodeling in engineering design" by Xiong et. al.
## -------------------------------------------------
## This function implements the nonstationary version of the correlation
## function.
function r = scf_nonst_corr(flm, fln)
  if (nargin < 2)
    usage("scf_nonst_corr(flm, fln)");
  endif

  if (columns(flm) != columns(fln))
    error("The vectors flm and fln have to have the same dimension.");
  endif

  exp = (flm - fln).^2;
  s = sum(exp);
  r = e^(-s);
endfunction


function R = scf_nonst_m(X, xi, eta, nugget)
  if (nargin != 4)
    usage("scf_nonst_m(X, xi, eta, nugget)");
  endif

  r = ones(rows(X), 1);
  r = r + nugget;
  R = diag(r);
  flm = zeros(1, columns(X));
  fln = zeros(1, columns(X));

  for i = 1:rows(R)
    for j = i+1:columns(R)
      for l = 1:columns(X)
        flm(l) = mapping_func(X(i,l), xi(:,l), eta(:,l));
        fln(l) = mapping_func(X(j,l), xi(:,l), eta(:,l));
      endfor
      R(i, j) = scf_nonst_corr(flm, fln);
      R(j, i) = R(i, j);
    endfor
  endfor
endfunction


function glk = density_knot(xl, xil, etal, k_up)
  glk = 0;

  isInRange = ((xl <= xil(k_up)) & (xl >= xil((k_up) - 1)));

  alk = (xil(k_up) .* etal((k_up)-1) - xil((k_up)-1) .* etal(k_up)) .* \
      (xil(k_up) - xil((k_up)-1)).^-1;
  blk = (etal(k_up) - etal((k_up)-1)) .* (xil(k_up) - xil((k_up)-1)).^-1;;
  glk = (alk + blk .* xl) .* (isInRange);
endfunction


function glk = density_knot_integr(xl, xm, xil, etal, k_up)
  glk = 0;

  alk = (xil(k_up) .* etal((k_up)-1) - xil((k_up)-1) .* etal(k_up)) .\ \
      (xil(k_up) - xil((k_up)-1));
  blk = (etal(k_up) - etal((k_up)-1)) .\ (xil(k_up) - xil(k_up-1));
  glk = (alk .* xl + blk .* 1 / 2 .* xl.^2);
  glm = (alk .* xm + blk .* 1 / 2 .* xm.^2);

  glk = glm - glk;
endfunction


function glk = density_knot_integr2(xl, xm, etal, k_up)
  glk = (etal(k_up-1) .* xm + etal(k_up) .* 1 / 2 .* xm.^2);
  glm = (etal(k_up-1) .* xl + etal(k_up) .* 1 / 2 .* xl.^2);

  glk = glk - glm;
endfunction


function gl = density(xl, xil, etal)
    gl = sum(density_knot(xl, xil, etal, 2:rows(xil)));
endfunction


function glk = density_knot_integr2_deriv(xl, xm, etal, curL, curK, l, k)
  glk = glm = 0;

  if (curL == l)
    if (k == (curK - 1))
      glk = xm;
      glm = xl;
    elseif (k == curK)
      glk = xm * eta(curK);
      glm = xl * eta(curK - 1);
    endif
  endif

  glk = glk - glm;
endfunction



## curl is the current dimension
## l identifies the derivate of the l-th dimension
## k identifies the derivate on the k-th knot
function fl = mapping_func_deriv(xl, xil, etal, curL, l, k)
  fl = 0;
  curK = 1;

  for curK = 2:rows(xil)
    if (xl < xil(curK))
      break;
    else
      fl += density_knot_integr2_deriv(xil(curK-1), xil(curK), etal, \
                                       curL, curK, l, k);
    endif
  endfor

  fl += density_knot_integr2_deriv(xil(curK-1), xl, etal, curL, curK, l, k);
endfunction


function R = scf_nonst_m_deriv(X, eta, xi, l, k, nugget)
  r = ones(rows(X), 1);
  r = r + nugget;
  R = diag(r);
  flm = zeros(1, columns(X));
  fln = zeros(1, columns(X));

  for i = 1:rows(R)
    for j = i+1:columns(R)
      for curL = 1:columns(X)
        flm(curL) = mapping_func_deriv(X(i,curL), xi(:,curL), \
                                       eta(:,curL), curL, l, k);
        fln(curL) = mapping_func_deriv(X(j,curL), xi(:,curL), eta(:,curL), \
                                       curL, l, k);
      endfor
      R(i, j) = scf_nonst_corr(flm, fln);
      R(j, i) = R(i, j);
    endfor
  endfor
endfunction


function fl = mapping_func(xl, xil, etal)
  fl = xil(1);
  k = 1;

  for k = 2:rows(xil)
    if (xl < xil(k))
      break;
    else
      fl += density_knot_integr2(xil(k-1), xil(k), etal, k);
    endif
  endfor
  fl += density_knot_integr2(xil(k-1), xl, etal, k);
endfunction


function r = scf_nonst_u(X, x, xi, eta)
  if (nargin != 4)
    usage("scf_nonst_u(X, x, xi, eta)");
  endif

  if (columns(x) != columns(X))
    error("The vector x has to have the same dimension as matrix columns.");
  endif

  r = zeros(rows(X), 1);

  for l = 1:columns(X)
    flx(l) = mapping_func(x(l), xi(:,l), eta(:,l));
  endfor

  for i = 1:rows(X)
    for l = 1:columns(X)
      fln(l) = mapping_func(X(i,l), xi(:,l), eta(:,l));
    endfor
    r(i,:) = scf_nonst_corr(flx, fln);
  endfor
endfunction
