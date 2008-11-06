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
## Keywords: gamma distribution, random number generation
## Created: 30.10.2008
## Version: 0.1


function gs = gamma_rnds(sigma, alpha, n)
  gs = zeros(1, n);
  for i = 1:n
    gs(i) = gamma_rnd(sigma, alpha);
  endfor
endfunction


function persist_gamma(g, file)
  fd = fopen(file, "wt");
  fprintf(fd, "gamma\n");
  fprintf(fd, "%.10f\n", g);
  fclose(fd);
endfunction

function g = gamma_rnd(a)
  b = c = g = X = Y = Z = U = V = W = 0;

  if (a<0.00001)
    X = a;
  elseif (a<=1) 
    U = rand();
    X = gamma_rnd(1+a) * U^(1/a);
  elseif (a<1.00001)
    X = -log(rand()); 
  else
    b = a-1;
    c = 3*a - 0.75;
    
    while (1)
      U = rand();
      V = rand();
      
      W = U*(1-U);
      Y = sqrt(c/W) * (U-0.5);
      X = b+Y;
      
      if (X>=0)
        Z = 64*W*W*W*V*V;
	
        if (Z <= 1 - 2*Y*Y/X || log(Z) <= 2 * (b*log(X/b) - Y))
	  break;
	endif
      endif
    endwhile
  endif

  # X<1e-30 && X<a ? (a<1e-30 ? a : 1e-30) : X;
  if (X<1e-30 && X<a)
    if (a<1e-30)
      g = a;
    else
      g = 1e-30;
    endif
  else
    g = X;
  endif
endfunction
