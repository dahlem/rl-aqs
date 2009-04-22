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
## Keywords: gather q values
## Created: 15.04.2009


## this script needs to be executed in the simulation directory
## it gathers the q-values of all graphs at the end of each replication

library(igraph)

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)

df <- data.frame(utilisationMean=rep(0, max(simulations$sim_num)),
                 utilisationStdDev=rep(0, max(simulations$sim_num)));

## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs

  dfreps <- data.frame(utilisations=rep(0, simulations[simulations$sim_num == sim,]$actual_reps));
  
  ## (2) for all replications
  for (i in 1:(simulations[simulations$sim_num == sim,]$actual_reps)) {
    prefix <- paste(sim, "/", i, "/", sep="")
    lastGraphFile <- paste(prefix, "graphs/graph", (graphs - 1), ".gml", sep="")

    ## (3) read graph
    print(paste("Read", lastGraphFile))
    graph <- read.graph(lastGraphFile, format="graphml")

    ## (4) mean utli
    utilisation <- V(graph)$utilisation
    dfreps$utilisations[i] <- sum(utilisation)/simulations$network_size
  }

  ## (5) take mean
  df$utilisationMean[sim] <- mean(dfreps$utilisations)
  df$utilisationStdDev[sim] <- sd(dfreps$utilisations)
}
  
## (6) write mean, std.dev. into "utilisation-values.dat"
write.table(df, "utilisation-values.dat", sep=",", row.names=FALSE, append=TRUE, quote=FALSE)
