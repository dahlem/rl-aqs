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
## Keywords: utilities, power law
## Created: 11.02.2009

is.discrete <- function(x, tol = .Machine$double.eps) {
  (x - floor(x)) < tol
}


des.power.law.dist <- function(x) {
  ## remove zeros, because logs can't deal with them
  x <- x[x>0]
  n <- length(x)

  ## is.discrete part of plyr
  if (all(is.discrete(x))) {
    xsorted <- sort(x)
    vals <- unique(x)
    cumFreq <- tabulate(match(xsorted, vals))/n
  } else {
    vals <- sort(x)
    cumFreq <- seq(n, 1)/n
  }

  return(list(x=vals, cumFreq=cumFreq))
}
