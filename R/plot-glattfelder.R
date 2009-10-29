## Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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



library(desGraph)
library(desInfluence)

message("Start with the analysis of dynamic behaviour!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)

## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  vertices <- simulations[simulations$sim_num == sim,]$network_size
  interval <- stopTime / (graphs - 1);

  ## (2) plot of glattfelders control metrics
  graph1 <- read.graph(paste(sim, "/1/graphs/graph", 0, ".gml", sep=""), format="graphml")
  prop1 <- des.qval.average.props(graph1)
  df1 <- des.control.ownership(graph1, policy="wpl", prop1)
  des.control.plot("start-", df1)
  
  graph2 <- read.graph(paste(sim, "/1/graphs/graph", graphs-1, ".gml", sep=""), format="graphml")
  prop2 <- des.qval.average.props(graph2)
  df2 <- des.control.ownership(graph2, policy="wpl", prop1)
  des.control.plot("end-", df2)
}


message("Finished.")
