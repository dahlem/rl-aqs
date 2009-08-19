function w = projSimplex(v, z, b)
  n = columns(v);
  U = 1:n;
  s = 0;
  rho = 0;
  
  while (columns(U) > 0)
    U
    ## pick k in U at random
    k = unidrnd(columns(U))

    ## partition U
    G = U(U >= U(k))
    L = U(U < U(k))

    delta_rho = columns(G)
    delta_s = sum(v(G))

    if ((s + delta_s) - (rho + delta_rho) * v(U(k)) < z)
      s = s + delta_s
      rho = rho + delta_rho
      U = L;
    else
      U(k) = [];
    endif
  endwhile

  theta = (s - z) / rho
  w = max(v - theta, b);
endfunction
