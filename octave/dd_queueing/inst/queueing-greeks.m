## Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: Utilisation vs. Mean total delay, greeks, Delta, Gamma
## Created: 15.01.2009
## Version: 0.1

function f = differentiate(f1, f2, h)
  f = (f2 - f1) / (2 * h);
endfunction


function calcGreeks(prefix, X, yU, yD, min, max, betas, thetas, step, nugget, FUN)
  betaU = [];
  for i = 1:betas
    file = [prefix "-Rho-chain-beta_" int2str(i) "-sampled.dat"];
    temp = csvread(file);
    temp = temp(2:end,:);
    betaU = [betaU; mean(temp)];
  endfor

  betaD = [];
  for i = 1:betas
    file = [prefix "-W-chain-beta_" int2str(i) "-sampled.dat"];
    temp = csvread(file);
    temp = temp(2:end,:);
    betaD = [betaD; mean(temp)];
  endfor

  thetaU = [];
  for i = 1:thetas
    file = [prefix "-Rho-chain-theta" int2str(i) "-sampled.dat"];
    temp = csvread(file);
    temp = temp(2:end,:);
    thetaU = [thetaU, mean(temp)];
  endfor

  thetaD = [];
  for i = 1:thetas
    file = [prefix "-W-chain-theta" int2str(i) "-sampled.dat"];
    temp = csvread(file);
    temp = temp(2:end,:);
    thetaD = [thetaD, mean(temp)];
  endfor

  RU = scf_gaussianm(X, thetaU, nugget);
  RD = scf_gaussianm(X, thetaD, nugget);

  greeksDelay(prefix, X, yU, yD, thetaU, thetaD, betaU, betaD, RU, RD, \
              step, min, max, FUN);
endfunction


function greeksDelay(prefix, X, yU, yD, thetaU, thetaD, betaU, \
                     betaD, RU, RD, step, min, max, FUN)
  ## write the surrogate details
  surrogateDetailDelay(prefix, X, yU, yD, thetaU, thetaD, betaU, \
                       betaD, RU, RD, step, min, max, FUN);

  ## calculate the Delta and Gamma
  surrogate = [prefix "-surrogate-details-delay.dat"];
  D = greeksDeltaUtilisationDelay(surrogate);
  G = greeksGammaUtilisation(surrogate, D);
  samples = (max(1) - min(1)) / step;
  
  ## write the output into a gnuplot friendly file
  data = csvread(surrogate);
  [r,c] = size(data);

  outfile = [prefix "-surrogate-greeks-details-delay.dat"];
  fd = fopen(outfile, "wt");
  for i = (3:(r - 2))
    fprintf(fd, "%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n",
	    data(i, 1), data(i, 2), data(i, 3), data(i, 4), D(i), G(i));
    if ((i - samples - 1) < 0)
      if ((i+1) == int16(samples))
	fprintf(fd, "\n");
      endif
    else
      if (mod(i,int16(samples-1)) == 0)
	fprintf(fd, "\n");
      endif
    endif
  endfor
  fclose(fd)
endfunction


function surrogateDetailDelay(prefix, X, yU, yD, thetaU, thetaD, betaU, \
                              betaD, RU, RD, step, min, max, FUN)
  samples = (max(1) - min(1)) / step;
  x1_vec = linspace(min(1), max(1), samples);
  x2_vec = linspace(min(2), max(2), samples);

  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor

  outfile = [prefix "-surrogate-details-delay.dat"];
  fd = fopen(outfile, "wt");
  for x1 = x1_vec
    for x2 = x2_vec
      krigU = krig([x1, x2], X, RU, betaU, thetaU, yU, F, FUN);
      krigD = krig([x1, x2], X, RD, betaD, thetaD, yD, F, FUN);
      fprintf(fd, "%.10f,%.10f,%.10f,%.10f\n", x1, x2, krigU, krigD);
    endfor
  endfor
  fclose(fd);
endfunction


function surrogateDetailJobs(prefix, X, yU, yL, thetaU, thetaL, betaU, \
                             betaL, RU, RL, step, min, max, FUN)
  samples = (max(1) - min(1)) / step;
  x1_vec = linspace(min(1), max(1), samples);
  x2_vec = linspace(min(2), max(2), samples);

  F = [];
  for i = 1:rows(X)
    F = [F; FUN(X(i,:))];
  endfor

  outfile = [prefix "-surrogate-details-jobs.dat"];
  fd = fopen(outfile, "wt");
  for x1 = x1_vec
    for x2 = x2_vec
      krigU = krig([x1, x2], X, RU, betaU, thetaU, yU, F, FUN);
      krigL = krig([x1, x2], X, RL, betaL, thetaL, yL, F, FUN);
      fprintf(fd, "%.10f,%.10f,%.10f,%.10f\n", x1, x2, krigU, krigL);
    endfor
    fprintf(fd, "\n");
  endfor
  fclose(fd);
endfunction


function D = greeksDeltaUtilisationDelay(surrogate)
  data = csvread(surrogate);
  [r,c] = size(data);

  ## the array is 2 elements smaller than the number of rows in data,
  ## because we use the central difference scheme
  D = zeros(0, r);

  for i = (2:(r-1))
    ## calculate the step size (it is not constant)
    h = abs((data((i+1), 3) - data((i-1), 3)) / 2);
    D(i) = differentiate(data((i-1), 4), data((i+1), 4), h);
  endfor
endfunction


function D = greeksDeltaUtilisationJobs(surrogate)
  data = csvread(surrogate);
  [r,c] = size(data);

  ## the array is 2 elements smaller than the number of rows in data,
  ## because we use the central difference scheme
  D = zeros(0, r);

  for i = (2:(r-1))
    ## calculate the step size (it is not constant)
    h = abs((data((i+1), 3) - data((i-1), 3)) / 2);
    D(i) = differentiate(data((i-1), 4), data((i+1), 4), h);
  endfor
endfunction


function G = greeksGammaUtilisation(surrogate, D)
  data = csvread(surrogate);
  [r,c] = size(data);

  ## the array is 4 elements smaller than the number of rows in data,
  ## because we use the second derivative using the central difference scheme
  G = zeros(0, r);

  for i = (3:(r-2))
    ## calculate the step size (it is not constant)
    h = abs((data((i+1), 3) - data((i-1), 3)) / 2);
    G(i) = differentiate(D(i-1), D(i+1), h);
  endfor
endfunction
