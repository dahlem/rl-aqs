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
## Keywords: Kriging, 2D Six-Hump Camelback Function
## Created: 22.01.2008
## Version: 0.1

global X = [
	    -2.0, 0.0;
	    -1.8, -0.7;
	    -1.6, 0.5;
	    -1.4, 1.0;
	    -1.2, -0.3;
	    -1.0, 0.2;
	    -0.8, -0.8;
	    -0.6, 0.7;
	    -0.4, -0.4;
	    -0.2, 0.1;
	    0.0, -0.9;
	    0.2, 0.9;
	    0.4, 0.4;
	    0.6, -0.5;
	    0.8, -0.1;
	    1.0, -1.0;
	    1.2, 0.8;
	    1.4, 0.3;
	    1.6, -0.6;
	    1.8, -0.2;
	    2.0, 0.6
	    ];

function y = shdf(x1, x2)
  if ((x1 < -2) || (x1 > 2))
    error("x1 of the shcf function is defined over [-2;2]");
  endif
  if ((x2 < -1) || (x2 > 1))
    error("x2 of the shcf function is defined over [-2;2]");
  endif
  
  y = (4 - 2.1 * x1^2 + x1^4/3) * x1^2 + x1 * x2 + (-4 + 4 * x2^2) * x2^2;
endfunction


function shdf_jaydmartin()
  global X;
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  outfile = "../doc/2d-shdf-jdm.dat";

  points = [];
  
  for x1 = x1_vec
    for x2 = x2_vec
      points = [points, x1, x2, shdf(x1, x2)];
    endfor
  endfor

  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f %14.6f %14.6f\n", points);
  fclose(fd);
  
endfunction


function points = shdf_real()
  outfile = "../doc/2d-shdf-real.dat";
  samples = 100;

  x1_vec = linspace(-2, 2, samples);
  x2_vec = linspace(-1, 1, samples);

  points = [];
  
  for x1 = x1_vec
    for x2 = x2_vec
      points = [points, x1, x2, shdf(x1, x2)];
    endfor
  endfor

  fd = fopen(outfile, "wt");
  fprintf(fd, "%14.6f %14.6f %14.6f\n", points);
  fclose(fd);
    
endfunction


function chain = shdf_krig()
  global X;
  
  x1_vec = X(:,1)';
  x2_vec = X(:,2)';

  y = [];
  
  for i = 1:columns(x1_vec)
    y = [y; shdf(x1_vec(i), x2_vec(i))];
  endfor
  
  # mcmc_mh(s1, x0_beta, x0_theta, x0_sigma, X, y, n)
  chain = mcmc_mh(50, 2, [1, 1], 2, X, y, 10000);
  
  
endfunction
