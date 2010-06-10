## Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: plotting
## Created: 15.07.2008



library(desStats)
library(desGraph)


message("Start with the analysis of dynamic behaviour!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)

## (1) for all simulations
for (sim in simulations$sim_num) {
  numGraphs <- simulations[simulations$sim_num == sim,]$graphs - 1
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  reps <- simulations[simulations$sim_num == sim,]$actual_reps

  graphs <- paste(sim, "/", 1, "/graphs", "/graph", seq(0,numGraphs), ".gml", sep="")
  dists <- sapply(graphs, des.read.graph.info, 2)

  df <- data.frame(time=rep(0, numGraphs+1), dist=rep(0, numGraphs+1))
  df$dist = dists

  for (rep in 2:reps) {
    graphs <- paste(sim, "/", rep, "/graphs", "/graph", seq(0,numGraphs), ".gml", sep="")
    dists <- sapply(graphs, des.read.graph.info, 2)

    df <- cbind(df, dists)
    print(paste("Finished ", rep, "replication"))
  }
  
  muDist <- rowMeans(df)
  seDist <- qt(0.975, df=reps-1) * apply(df, 1, sd) / sqrt(reps)

  interval <- stopTime / numGraphs
  results <- data.frame(time=seq(0, numGraphs) * interval, dist=muDist, seDist, dist1=dists)
  
  write.csv(results, "graph-info-distance-from-rand-evo-plot.dat", quote=F, row.names=F)
}


message("Finished.")
