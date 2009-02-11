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
## Keywords: utilities, correlation
## Created: 11.02.2009

des.corr <- function(x, y) {
  m1 <- mean(x)
  m2 <- mean(y)
  m12 <- mean(x*y)
  m11 <- mean(x^2)
  m22 <- mean(y^2)

  corr <- (m12 - m1*m2) / sqrt((m11 - m1^2) * (m22 - m2^2))

  return(corr)
}
