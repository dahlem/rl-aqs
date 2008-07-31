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
## Keywords: utilities, merge data frames
## Created: 18.04.2008


merge.result <- function(result) {
  data <- result[[1]]
  for (i in 2:length(result)) {
    data <- base::merge(data, result[[i]], all=TRUE, sort=FALSE)
  }

  return(data)
}


equal.space <- function(min, max, number) {
  return(seq(min, max, (max-min)/(number-1)))
}
