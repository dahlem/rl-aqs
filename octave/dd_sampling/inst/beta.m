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
## Keywords: beta-process sampling
## Created: 08.04.2008
## Version: 0.1

function BP = beta_process(epsilon, m, T)
  X = [];
  BP = [0];

  pp = nhpp(epsilon, m, T);

  for i = 1:pp.I
    X = [X; betarnd(epsilon, m * pp.S(i))];
  endfor
  for i = 1:pp.I
    if (X(i) >= BP(i))
      BP = [BP; X(i)];
    else
      BP = [BP; BP(i)];
    endif
  endfor
  BP = [BP; 1];
endfunction


function plot_path(G)
  plot(G(:), "color", [0,0,0])
##  line(1:rows(G.alpha), G.alpha, "color", [1,0,0]);
##  line(1:rows(G.beta), G.beta, "color", [0,0,1]);
endfunction
