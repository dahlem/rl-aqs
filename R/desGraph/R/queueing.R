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
## Keywords: queueing networks, jackson
## Created: 09.01.2009


des.queueing <- function(graph) {
  ## external arrival rate for node i
  gamma <- get.vertex.attribute(graph, "arrival_rate")

  ## transission probabilities from node i to node j
  Q <- des.queueing.Q.matrix(graph)
  I <- diag(vcount(graph))
  temp <- I - Q

  ## total arrival rate for node i
  lambda <- gamma %*% solve(temp)

  ## service rate
  mu <- V(graph)$service_rate

  ## utilisation
  rho <- lambda / mu

  ## number of jobs at node i
  L <- rho / (1 - rho)

  ## mean total waiting time
  W <- 1 / sum(gamma) * sum(L)

  ## average response time
  w <- 1 / (mu - lambda)

  ## average waiting time
  r <- w %*% solve(temp)

  ## average number of times a job visits node i
  upsilon <- lambda / sum(gamma)

  ## jackson product form
  pfi <- (1 - rho) * rho^L
  pf <- prod(pfi)

  return(list(gamma=gamma, mu=mu, lambda=lambda, rho=rho,
              L=L, W=W, w=w, r=r, upsilon=upsilon, jackson.pf=pf))
}


des.queueing.jackson.pf <- function(graph) {
  L <- des.queueing.L.vec(graph)
  rho <- des.queueing.rho.vec(graph)
  pfi <- (1 - rho) * rho^L

  return(prod(pfi))
}

## average number of times that a job visits node i
des.queueing.upsilon.vec <- function(graph, Q=NULL) {
  gamma <- get.vertex.attribute(graph, "arrival_rate")
  gamma1 <- gamma / sum(gamma)
  if (is.null(Q)) {
    Q <- des.queueing.Q.matrix(graph)
  }
  I <- diag(vcount(graph))
  temp <- I - Q
  upsilon <- gamma1 %*% solve(temp)

  return(upsilon)
}

## average number of times that a job visits node i
des.queueing.upsilon.vec1 <- function(graph, Q=NULL) {
  gamma <- get.vertex.attribute(graph, "arrival_rate")
  lambda <- des.queueing.lambda.vec(graph, Q)
  upsilon <- lambda / sum(gamma)
  
  return(upsilon)
}

## average waiting time
des.queueing.r.vec <- function(graph, Q=NULL) {
  if (is.null(Q)) {
    Q <- des.queueing.Q.matrix(graph)
  }
  I <- diag(vcount(graph))
  w <- des.queueing.w.vec(graph, Q)
  temp <- I - Q
  r <- w %*% solve(temp)

  return(r)
}

## average response times
des.queueing.w.vec <- function(graph, Q=NULL) {
  lambda <- des.queueing.lambda.vec(graph, Q)
  mu <- V(graph)$service_rate
  w <- 1 / (mu - lambda)
  
  return(w)
}

## retuns waiting time
des.queueing.W.vec <- function(graph, Q=NULL) {
  gamma <- get.vertex.attribute(graph, "arrival_rate")
  L <- des.queueing.L(graph, Q)
  W <- 1 / sum(gamma) * L
}

## return mean number of jobs in the system
des.queueing.L <- function(graph, Q=NULL) {
  L <- sum(des.queueing.L.vec(graph, Q))

  return(L)
}

## returns number of jobs at vertex i
des.queueing.L.vec <- function(graph, Q=NULL) {
  rho <- des.queueing.rho.vec(graph, Q)
  L <- rho / (1 - rho)

  return(L)
}

## returns utilisation
des.queueing.rho.vec <- function(graph, Q=NULL) {
  lambda <- des.queueing.lambda.vec(graph, Q)
  mu <- V(graph)$service_rate
  rho <- lambda / mu

  return(rho)
}

## returns total arrival rate
des.queueing.lambda.vec <- function(graph, Q=NULL) {
  gamma <- get.vertex.attribute(graph, "arrival_rate")
  if (is.null(Q)) {
    Q <- des.queueing.Q.matrix(graph)
  }
  I <- diag(vcount(graph))
  temp <- I - Q
  lambda <- gamma %*% solve(temp)

  return(lambda)
}

## returns transition probabilities
des.queueing.Q.matrix <- function(graph) {
  vertices <- vcount(graph)
  Q <- matrix(rep(0, vertices^2), nrow=vertices, ncol=vertices)

  for (outV in V(graph)) {
    for (inV in neighbors(graph, outV, mode="out")) {
      Q[(V(graph)$id[outV + 1] + 1), (V(graph)$id[inV + 1] + 1)] <- E(graph)[from(outV) & to(inV)]$weight
    }
  }

  return(Q)
}
