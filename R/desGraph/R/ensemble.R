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
  P <- get.adjacency(graph, attr="weight")
  P <- P / rowSums(P)
  P[is.nan(P)] <- 0

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
  knni <- sum(P[i,] * P) / sum(P[i,])
  return(knni)
}

des.ensemble.average.degree.neighbours <- function(P) {
  knn <- sapply(1:nrow(P), des.ensemble.average.degree.neighbours.i, P)
}

des.ensemble.cluster.i <- function(i, P) {
  numerator <- 0.0
  denominator <- 0.0

  for (j in 1:nrow(P)) {
    for (k in 1:nrow(P)) {
      if ((k != j) && (j != i) && (i != k)) {
        temp <- P[i, j] * P[i, k]
        numerator <- numerator + temp * P[j, k]
        denominator <- denominator + temp
      }
    }
  }

  ci <- numerator / denominator
  return(ci)
}

des.ensemble.cluster <- function(P) {
  c <- sapply(1:nrow(P), des.ensemble.cluster.i, P)
  return(c)
}

des.ensemble.distance.beta <- function(A, i, j, m) {
  
}

des.ensemble.distance.alpha <- function(A, i, j, m) {
}
