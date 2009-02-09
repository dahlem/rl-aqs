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
## Keywords: independent component analysis
## Created: 09.02.2009


## mode 1: based on qval
##      2: based on weight
##      3: based on shortest paths
des.ica <- function(graphs, mode) {
  X <- matrix()
  vertices <- c()
  
  for (i in 1:length(graphs)) {
    graphInfo <- des.graph.info(graphs[[i]], mode=mode, TRUE)

    if (i == 1) {
      ## allocate enough columns for each vertex we are interested in
      X <- matrix(graphInfo$distance, ncol=length(graphInfo$distance))
      vertices <- graphInfo$vertices
    } else {
      X <- rbind(X, graphInfo$distance)
    }
  }

  return(list(X=X, vertices=vertices))
}
