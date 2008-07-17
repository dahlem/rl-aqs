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
## Keywords: graph, analysis, small-world properties
## Created: 16.07.2008

des.graph.sw <- function(graph) {
  average.path <- average.path.length(graph)
  diameter <- diameter(graph)
  order <- log(vcount(graph))
  cluster.coefficient <- des.graph.cluster(graph)

  return(data.frame(average.path,
                    diameter,
                    order,
                    cluster.coefficient))
}


des.graph.cluster.local <- function(graph) {
  cs <- rep(NaN, vcount(graph))
  
  for (v in V(graph)) {
    # calculate the local neighbourhoods
    vertices <- neighborhood.size(graph, 1, v, mode="out")
    
    # calculate the total in+out degree of a given vertex
    if (vertices > 1) {
      e <- vertices - 1
      n <- neighborhood(graph, 1, v, mode="out")[[1]]
      g <- subgraph(graph, n[-1])
      cs[v+1] <- ecount(g) / (e * (e - 1))
    }
  }

  return(cs)
}


des.graph.cluster <- function(graph) {
  return(mean(des.graph.cluster.local(graph), na.rm=TRUE))
}
