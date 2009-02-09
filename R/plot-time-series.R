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
