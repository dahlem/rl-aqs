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

library(igraph)
library(ggplot2)
library(desStats)
library(graphProps)

message("Start with the analysis of neural network diagnostics!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)


## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  numReps <- simulations[simulations$sim_num == sim,]$actual_reps
  numGraphs <- (graphs - 1);

  interval <- stopTime / numGraphs;
  time = seq(0, numGraphs) * interval
  dir <- sim

  df <- get.var.snapshots(dir, numGraphs, numReps, "regret_absolute", ci=0.975, mode="total")
  df$time <- time
  write.csv(df, paste(sim, "-regret.csv", sep=""))
  plot.var(paste(sim, "-regret.csv", sep=""), paste(sim, "-nn-mse.pdf", sep=""), xtitle="Time", ytitle="Total MSE", breaks=NULL, se=T, ps=T)
}


message("Finished.")
