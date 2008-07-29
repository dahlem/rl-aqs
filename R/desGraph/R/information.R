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
## Keywords: graph, analysis, degree, information-theory
## Created: 24.07.2008

des.graph.max.info <- function(graph) {
  return(mean(des.graph.node.max.info(graph)))
}


des.graph.node.max.info <- function(graph, vertex=V(graph)) {
  outdegree <- degree(graph, v=vertex, mode="out", loops=FALSE)
  info <- des.graph.max.info.fn(outdegree)

  # replace -Inf with zeros
  info[info == -Inf] <- 0
  
  return(info)
}


des.graph.max.info.fn <- function(outdegree) {
  return(2 - 2/outdegree)
}


des.graph.info <- function(graph) {
  # for each vertex calculate the available information using
  # the weights of all out-edges
  
  return(mean(sapply(V(graph), des.graph.node.info, graph=graph)))
}


des.graph.node.info <- function(vertex, graph) {
  weights <- E(graph)[from(vertex)]$weight
  outdegree <- 1/length(weights)
  
  return(des.graph.info.fn(outdegree, weights))
}


des.graph.info.fn <- function(outdegree, weights) {
  diff <- outdegree - weights
  return(sum(abs(diff)))
}
