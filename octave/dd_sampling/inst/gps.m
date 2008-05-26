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
## Keywords: Gamma Sequential Sampling
## Created: 09.02.2008
## Version: 0.1

## G = gpsBridge(y, m, T, k)
## gamma bridge processes
## Input:
##  m  : scale parameter
##  T  : max time T
##  k  : number of intervals 2^k
function G = gpsBridgeDebug(m, T, k)
  G.X = [0,0];
  G.alpha = [];
  G.beta = [];
  h = (2^-k) * T;

  for i = 2:2^k
    t = (i - 1) * h;
    G.alpha = [G.alpha; m * t];
    G.beta = [G.beta; m * (T - t)];
    Q = betarnd(m * t, m * (T - t));
    if (Q < G.X(i - 1, 2))
      Q = G.X(i - 1, 2);
    endif
    g = [t, Q];
    G.X = [G.X; g];
  endfor

  G.X = [G.X; [T, 1]];
endfunction


function G = gpsBridge(m, T, k)
  G.X = [0,0];
  h = (2^-k) * T;

  for i = 2:2^k
    t = (i - 1) * h;
    Q = betarnd(m * t, m * (T - t));
    if (Q < G.X(i - 1, 2))
      Q = G.X(i - 1, 2);
    endif
    g = [t, Q];
    G.X = [G.X; g];
  endfor

  G.X = [G.X; [T, 1]];
endfunction


function plotBridge(G)
  plot(G.X(:,2), "color", [0,0,0])
#  line(1:rows(G.alpha), G.alpha, "color", [1,0,0]);
#  line(1:rows(G.beta), G.beta, "color", [0,0,1]);
endfunction

function p = testBeta(a, b, k)
  p = [0];
  for i = 2:k
    p = [p; betarnd(a, b)];
  endfor
endfunction