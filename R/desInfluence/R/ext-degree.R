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
## Keywords: influence, degree, weighted-directed network
## Created: 22.04.2009
## Based on: The backbone of complex networks in corporations: Who is controlling
##           whom? by Glattfelder and Battiston

## calculate the influence strength acc. to equation (1)
## wrt queueuing theory the weight is in fact the aggregate arrival * probability of routing
## to the destination node + external arrival
des.influence.weight.i <- function(vertexFrom, vertexTo, graph, Q, lambda) {
  ## calculate the weight of the edge connecting vertexFrom to vertexTo
  weight <- c(Q[(vertexFrom + 1), (vertexTo + 1)] * lambda[(vertexFrom + 1)],
              get.vertex.attribute(graph, "arrival_rate", index=vertexTo))
  
  return(weight)
}

## the sum of all incoming weights
des.influence.strength.in.i <- function(vertex, graph, Q, lambda) {
  inV <- neighbors(graph, vertex, mode="in")

  ## calculate the weight of the edge going from inV to vertex
  weights <- sapply(inV, des.influence.weight.i, vertexTo=vertex, graph=graph, Q=Q, lambda=lambda)

  if (length(weights) == 0) {
    weights = 0
  }
  
  ## sum up
  strength <- sum(weights, na.rm=T)
  return(strength)
}

des.influence.strength.in <- function(graph, Q, lambda) {
  strengths <- sapply(V(graph), des.influence.strength.in.i, graph=graph, Q=Q, lambda=lambda)
  return(strengths)
}

## calculate the concentration index acc. to equation (2)
des.concentration.index.i <- function(vertex, graph, Q, lambda) {
  inV <- neighbors(graph, vertex, mode="in")

  ## calculate the weight of the edge going from inV to vertex
  weights <- sapply(inV, des.influence.weight.i, vertexTo=vertex, graph=graph, Q=Q, lambda=lambda)

  if (length(weights) == 0) {
    weights <- 0
    concentration <- 0
  } else {
    concentration <- sum(weights, na.rm=T)^2/sum(weights^2, na.rm=T)
  }

  return(concentration)
}

des.concentration.index <- function(graph, Q, lambda) {
  concentrations <- sapply(V(graph), des.concentration.index.i, graph=graph, Q=Q, lambda=lambda)
  return(concentrations)
}


## calculate the control index acc. to equation (3)
des.control.index.i <- function(vertex, graph, Q, lambda) {
  inV <- neighbors(graph, vertex, mode="in")

  ## calculate the weight of the edge going from inV to vertex
  weights <- sapply(inV, des.influence.weight.i, vertexTo=vertex, graph=graph, Q=Q, lambda=lambda)

  if (length(weights) == 0) {
    weights <- 0
    control <- 0
  } else {
    control <- weights/sum(weights^2, na.rm=T)
  }

  return(sum(control, na.rm=T))
}

## calculate the control index acc. to equation (4)
des.control.index <- function(graph, Q, lambda) {
  controls <- sapply(V(graph), des.control.index.i, graph=graph, Q=Q, lambda=lambda)
  return(controls)
}


des.control.power.law <- function(prefix="", controls, ps=TRUE, width=7, height=7, pts=12) {
  if (ps) {
    filename <- paste(prefix, "control-index-power-law.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  res <- des.power.law.dist(controls)

  df <- data.frame(x=res$x, y=res$cumFreq)

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + geom_point()
  p <- p + coord_trans(x = "log", y = "log")
  p <- p + scale_y_continuous("P(h)")
  p <- p + scale_x_continuous("Control Index (h)")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.concentration.power.law <- function(prefix="", concentration, ps=TRUE, width=7, height=7, pts=12) {
  if (ps) {
    filename <- paste(prefix, "concentration-index-power-law.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  res <- des.power.law.dist(concentrations)

  df <- data.frame(x=res$x, y=res$cumFreq)

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + geom_point()
  p <- p + coord_trans(x = "log", y = "log")
  p <- p + scale_y_continuous("P(s)")
  p <- p + scale_x_continuous("Concentration Index (s)")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}
