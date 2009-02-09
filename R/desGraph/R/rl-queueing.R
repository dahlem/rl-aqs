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
## Keywords: queueing networks, RL
## Created: 08.02.2009


des.rl.epsilon.queueing.Q.matrix <- function(graph, epsilon) {
  vertices <- vcount(graph)
  Q <- matrix(rep(0, vertices^2), nrow=vertices, ncol=vertices)

  ## for all vertices
  for (outV in V(graph)) {
    neighbours <- neighbors(graph, outV, mode="out")

    if (length(neighbours) == 1) {
        Q[(V(graph)$id[outV + 1] + 1), (V(graph)$id[neighbours[1] + 1] + 1)] <- 1
    } else if (length(neighbours) > 1) {
      ## use structure to store inV and qvalues
      df <- data.frame(inV=neighbours, qvals=rep(0, length(neighbours)))

      ## fill the structure
      for (inV in 1:length(neighbours)) {
        df$qvals[inV] <- E(graph)[from(outV) & to(neighbours[inV])]$q_value
      }

      ## sort in descending order of the qvalues
      df <- df[order(df$qvals, decreasing=TRUE),] 

      ## calc epsilon for epsilon-greedy and assign the probabilities
      Q[(V(graph)$id[outV + 1] + 1), (V(graph)$id[neighbours[1] + 1] + 1)] <- (1 - epsilon)
      prob <- epsilon / (length(neighbours) - 1)

      for (i in 2:length(neighbours)) {
        Q[(V(graph)$id[outV + 1] + 1), (V(graph)$id[neighbours[i] + 1] + 1)] <- prob
      }
    }
  }

  return(Q)
}
