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
## Keywords: Non-homogeneous Poisson Process
## Created: 08.04.2008
## Version: 0.1

function l = lambda(epsilon, m, T, t)
  l = 1/(2 * epsilon) * m^2 * T^2 * t;
endfunction

function pp = nhpp(epsilon, m, T)
  t = 0;
  pp.I = 0;
  pp.S = [];
  lambda_star = lambda(epsilon, m, T, T);

  while (true)
    u = rand();
    t = t - 1/lambda_star * log(u);
    if (t > T)
      break;
    endif
    u = rand();
    if (u <= lambda(epsilon, m, T, t)/lambda_star)
      pp.I += 1;
      pp.S = [pp.S, t];
    endif
  endwhile
endfunction