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
## Keywords: time series analysis
## Created: 09.02.2009


des.ts.qval <- function(graphs) {
  lica <- des.ica(graphs, 1)
  ## remove the NaN rows from the matrix
  X <- lica$X[-unique(which(is.nan(lica$X),arr.ind=TRUE)[,1]),]
  fica <- fastICA(X, dim(X)[2], alg.typ="parallel", fun="exp")
  fica.som <- som(data=fica$A, rlen=2000, grid=somgrid(6,6,"hexagonal"))
  plot(fica.som, type="mapping", labels=lica$vertices)
}


des.ts.calcCorr <- function(Xdiff) {
  components <- ncol(Xdiff)
  R <- diag(components)

  for (i in 1:(components-1)) {
    for (j in (i+1):components) {
      R[i, j] <- corr(Xdiff[,c(i, j)])
      R[j, i] <- R[i, j]
    }
  }

  return(R)
}


des.ts.calcDist <- function(R) {
  n <- dim(R)[1]
  D <- matrix(rep(0, n*n), n, n)

  for (i in 1:(n-1)) {
    for (j in (i+1):n) {
##      D[i, j] <- sqrt(2 * (1 - R[i, j]))
      D[i, j] <- 1 - R[i, j]^2
      D[j, i] <- D[i, j]
    }
  }

  return(D)
}

des.adj.graph <- function(D) {
  ## remove nans
  D[is.nan(D)] <- 0
  graph <- graph.adjacency(D, mode="undirected", weighted=TRUE)

  return(graph)
}

des.mst <- function(graph) {
  mst <- minimum.spanning.tree(graph)
  return(mst)
}


des.layout <- function(mst, root=0) {
  layout <- layout.reingold.tilford(mst, circular=TRUE, root=root)
  layout1 <- layout.norm(layout, -1,1,-1,1)
  layout2 = layout.fruchterman.reingold.grid(mst, start=layout1,
    cellsize=vcount(mst)/2, root=root, repulserad=vcount(mst)^3)
  layout3 <- layout.norm(layout2, -1,1,-1,1)
  
  return(layout3)
}

plot.igraph(mst, layout=layout, vertex.label=vertexLabelSpec,
            vertex.size=10, vertex.size2=10, vertex.label.cex=0.8)


for (sim in simulations$sim_num) {
  replicas <- read.csv(paste(sim, "/replica_results.dat", sep=""), header=TRUE)
  numGraphs <- simulations[simulations$sim_num == sim,]$graphs
  
  for (i in replicas$rep_num) {
    prefix <- paste(sim, "/", i, "/", sep="")
    lgraph <- list()

    for (g in 1:(numGraphs - 1)) {
      graphFile <- paste(prefix, "graphs/graph", g, ".gml", sep="")
      graph <- read.graph(graphFile, format="graphml")
      lgraph[[length(lgraph)+1]] = graph
    }
  }
}
