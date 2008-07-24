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

des.graph.info <- function(graph) {
  return(mean(des.graph.node.info(graph)))
}


des.graph.node.info <- function(graph, vertex=V(graph)) {
  outdegree <- degree(graph, v=vertex, mode="out", loops=FALSE)
  outdegree <- outdegree[outdegree != 0]
  
  return(des.graph.max.info(outdegree))
}


des.graph.max.info <- function(outdegree, weight) {
  return(2 - 2/outdegree)
}
