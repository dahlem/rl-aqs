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
## Keywords: weighted graph analysis
## Created: 11.09.2009

des.topo.unweighted.undirected.cluster <- function(graphFiles) {
  cluster.coeffs <- c()
  
  for (g in graphFiles) {
    graph <- read.graph(g, format="graphml")
    cluster.coeffs <- rbind(cluster.coeffs, transitivity(g, type="global"))
  }

  return(cluster.coeffs)
}

des.topo.unweighted.avg.path <- function(graphFiles) {
  dists <- c()
  maxdists <- c()
  
  for (g in graphFiles) {
    graph <- read.graph(g, format="graphml")
    sp <- shortest.paths(graph, mode="out", weights=NA)
    sp[is.infinite(sp)] <- NA
    dists <- rbind(dists, average.path.length(graph, directed=TRUE))
    maxdists <- rbind(maxdists, max(sp, na.rm=T))
  }

  return(list(dists, maxdists))
}
