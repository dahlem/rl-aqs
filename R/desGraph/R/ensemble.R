## Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
##
## This file is free software; as a special exception the author gives
## unlimited permission to copy and/or distribute it, with or without
## modifications, as long as this notice is preserved.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
## implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## Author: Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie> Maintainer:
## Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie> Keywords: weighted graph
## analysis Created: 25.08.2009 Based on publication: "Ensemble
## approach to the analysis of weighted networks" by Ahnert et al.

des.ensemble.P <- function(graph) {
  Q <- des.queueing.Q.matrix(graph)
  lambda <- des.queueing.lambda.vec(graph, Q)

  W <- matrix(rep(as.matrix(lambda), vcount(graph)), ncol=vcount(graph), byrow=F) * Q

  P <- (W - min(W)) / (max(W) - min(W))

  return(P)
}

des.ensemble.A <- function(graph) {
  A <- get.adjacency(graph)
}

des.ensemble.strength <- function(P) {
  strength <- apply(P, 2, sum)
  return(strength)
}

des.ensemble.n <- function(P) {
  n <- sum(P)
  return(n)
}

des.ensemble.average.degree.neighbours.i <- function(i, P) {
  Pti <- P[i,]
  Pti[i] <- 0
  knni <- sum(Pt[i,] * P) / sum(Pt[i,])
  return(knni)
}

des.ensemble.average.degree.neighbours <- function(P) {
  knn <- sapply(1:nrow(P), des.ensemble.average.degree.neighbours.i, P)
}

des.ensemble.cluster.i <- function(i, P, graph) {
  edges <- length(which(P[i,] > 0, arr.ind=T))
##  edges <- degree(graph, v=(i-1), mode="total")
  cj <- 0

  if (edges > 1) {
    A <- matrix(P[i,], ncol=1) %*% matrix(P[i,], nrow=1)
    diag(A) <- 0
    A[i,] <- 0
    A[,i] <- 0

    Pt <- P
    diag(Pt) <- 0
    Pt[i,] <- 0
    Pt[,i] <- 0
    cj <- sum(A %*% Pt) / sum(A)
  }

  return(cj/vcount(graph))
}

des.ensemble.cluster <- function(P, graph) {
  c <- sapply(1:nrow(P), des.ensemble.cluster.i, P, graph)
  return(c)
}
}
