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
## Keywords: plotting, mst
## Created: 10.02.2009

## This script generates graphs for the steady-state. Those graphs include
## - weight distribution of msts (from "Minimum Spanning Trees on weighted scale-free networks",
##   by MacDonald, Almaas, Barabasi)

library(desGraph)


des.mst.weight.power.law.weight <- function(prefix, graph, name, neg=1, ps=TRUE, fit=FALSE, xtitle="") {
  if (ps) {
    postscript(paste(prefix, "mst-", name, "-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  weight <- get.edge.attribute(graph, name, index=E(graph))
  if (neg == 1) {
    weight <- -weight
  }

  mst <- minimum.spanning.tree(graph, weights=weight)
  
  df <- data.frame(weight=c(), cumFreq=c(), size=c())

  x <- get.edge.attribute(mst, name)
  if (neg == 1) {
    x <- -x
  }

  x <- x[x > 0,]
  res <- des.power.law.dist(x)

  df <- data.frame(x=res$x, y=res$cumFreq)

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + geom_point()
  if (fit) {
    pl <- plfit(df$x)
    dfPL <- data.frame(x=pl$xmin:max(df$x),y=10*(pl$xmin:max(df$x))^(-pl$alpha+1))
    dfPL <- dfPL[dfPL$y < max(df$y) & dfPL$y > min(df$y),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log", y = "log")
  p <- p + scale_y_continuous("Cumulative Frequencies")
  p <- p + scale_x_continuous(xtitle)
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.mst.weight.dist.weight <- function(prefix, graph, name, neg=1, ps=TRUE, fit=FALSE, xtitle="") {
  if (ps) {
    postscript(paste(prefix, "mst-", name, "-dist-plot.eps", sep=""), onefile=FALSE)
  }

  weight <- get.edge.attribute(graph, name, index=E(graph))
  if (neg == 1) {
    weight <- -weight
  }

  mst <- minimum.spanning.tree(graph, weights=weight)
  
  x <- get.edge.attribute(mst, name)
  if (neg == 1) {
    x <- -x
  }

  df <- data.frame(weight=sort(x), y=seq(1, length(x)))

  p <- ggplot(df, aes(x=weight, y=y))
  p <- p + geom_point()
  p <- p + scale_y_continuous("Cumulative Frequencies")
  p <- p + scale_x_continuous(xtitle)
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.influence.strength.power.law.weight <- function(q, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript("influence-strength-power-law.eps", onefile=FALSE)
  }

  x <- abs(q)
  res <- des.power.law.dist(x)

  df <- data.frame(x=res$x, y=res$cumFreq)

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + geom_point()
  if (fit) {
    pl <- plfit(df$x)
    dfPL <- data.frame(x=pl$xmin:max(df$x),y=10*(pl$xmin:max(df$x))^(-pl$alpha+1))
    dfPL <- dfPL[dfPL$y < max(df$y) & dfPL$y > min(df$y),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log", y = "log")
  p <- p + scale_y_continuous("Cumulative Frequencies")
  p <- p + scale_x_continuous(bquote(group("|", q, "|")))
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.influence.strength.row <- function(W) {
  W1 <- W
  
  ## make the diagonals zero
  diag(W1) <- 0
  
  q <- rowSums(W1)
  return(q)
}


des.influence.strength.col <- function(W) {
  W1 <- W
  
  ## make the diagonals zero
  diag(W1) <- 0
  
  q <- colSums(W1)
  return(q)
}


des.corr.mean <- function(G) {
  W <- diag(1, ncol(G))

  for (i in 1:(ncol(W)-1)) {
    for (j in (i+1):ncol(W)) {
      W[i, j] <- des.corr(G[,i], G[,j])
      W[j, i] <- W[i, j]
    }
  }

  return(W)
}


des.rel.return <- function(X) {
  G <- t(apply(X, 1, '-', colMeans(X)))
  return(G)
}


des.corr.graph.weight <- function(graph, G, degThres=TRUE) {
  W <- diag(1, ncol(G))
  df <- data.frame(vertices=as.vector(V(graph)), degree=degree(graph, mode="out"))

  if (degThres) {
    df <- df[df$degree > 1,]
  }
  
  for (i in df$vertices) {
    neighbours <- V(graph)[nei(i, mode="out")]
    ## we are only interested in learning agents
    if (length(neighbours) > 1) {
      for (j in neighbours) {
        ## we are only interested in learning agents
        neighboursneigh <- V(graph)[nei(j, mode="out")]
        if (length(neighboursneigh) > 1) {
          print(paste("From", i, "to", j))
          W[i+1, j+1] <- des.corr(G[,i+1], G[,j+1])
        }
      }
    }
  }

  return(W)
}




simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)

## do the graphs for each replica and all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  nodes <- simulations[simulations$sim_num == sim,]$network_size
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  
  for (i in simulations$actual_reps) {
    replicas <- read.csv(paste(sim, "/", i, "/replica_results.dat", sep=""), header=TRUE)
    prefix <- paste(sim, "/", i, "/", sep="")
    lastGraphFile <- paste(prefix, "graphs/graph", seq(1, (graphs - 1)), ".gml", sep="")
    graph <- read.graph(lastGraphFile, format="graphml")
  }
}
