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
## Keywords: graph, analysis, shortest-path
## Created: 28.07.2008

des.graph.shortest.paths <- function(graph) {
  g <- set.vertex.attribute(graph, "distance", value=Inf)

  for (v in V(g)) {
    ## get all shortest paths from this vertex
    shortestPaths <- get.shortest.paths(g, v, mode="out")

    ## check whether the targets are leaf nodes
    for (i in V(g)) {
      ## if it is, then set the distance vertex attribute
      if (length(shortestPaths[[i+1]]) > 0) {
        if (degree(g, i, mode="out", loops=FALSE) == 0) {
          sp <- length(shortestPaths[[i+1]]) - 1
          if (V(g)[v]$distance > sp) {
            V(g)[v]$distance = sp
          }
        }
      }
    }
  }

  return(g)
}


des.graph.shortest.paths.weights <- function(graph) {
  ## iterate through all vertices
  for (v in V(graph)) {
    ## find a neighbouring vertex with minimum distance attribute
    if (length(V(graph)[nei(v, mode="out")]) > 0) {
      minDist <- min(V(graph)[nei(v, mode="out")]$distance)

      ## set the edge weights connecting v and u
      E(graph)[from(v)]$weight <- 0

      ## select the first one, if there are more than one vertices
      for (u in V(graph)[nei(v, mode="out") & distance==minDist]) {
        E(graph)[from(v) & to(u)]$weight <- 1
        break
      }
    }
  }

  return(graph)
}


des.graph.calc.shortest.paths <- function(graph) {
  ## get all shortest paths
  sp <- shortest.paths(graph, mode="out", weights=NA)

  ## use -1 for all non-reachable paths
  sp[sp == Inf] = -1

  paths <- apply(sp, 1, max)
  return(paths)
}


des.graph.shortest.path.from <- function(graph, vertex) {
  neighbours <- V(graph)[nei(vertex, mode="out")]
  sp <- shortest.paths(graph, v=neighbours, mode="out", weights=NA)
  sp[sp == Inf] = -1
  paths <- 1 + apply(sp, 1, max)
}
