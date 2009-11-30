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

library(igraph)
library(ggplot2)
library(desStats)

message("Start with the analysis of regret!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)


des.read.incentive.deviate <- function(graphName) {
  graph <- read.graph(graphName, format="graphml")
  info <- V(graph)$incentive_deviate
  return(sum(info))
}


des.read.regret.absolute <- function(graphName) {
  graph <- read.graph(graphName, format="graphml")
  regretAbsolute <- V(graph)$regret_absolute
  numEvents <- V(graph)$num_events_processed
  return(sum(regretAbsolute / numEvents, na.rm=T))
}

des.plot.incentive.deviate <- function(dir, numGraphs, stopTime, ps=TRUE) {
  graphs <- paste(dir, "/graph", seq(0, numGraphs), ".gml", sep="")
  incentives <- sapply(graphs, des.read.incentive.deviate)

  interval <- stopTime / numGraphs;
  df <- data.frame(time=rep(0, numGraphs+1), incentives=rep(0, numGraphs+1))
  df$time = seq(0, numGraphs) * interval
  df$incentives = incentives

  if (ps) {
    des.postscript(paste(dir, "/../../../regret-incentive-deviate.eps", sep=""))
  }

  p <- ggplot(df, aes(x=time, y=incentives))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Incentive to Deviate")
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.plot.regret.absolute <- function(dir, numGraphs, stopTime, ps=TRUE) {
  graphs <- paste(dir, "/graph", seq(0, numGraphs), ".gml", sep="")
  regrets <- sapply(graphs, des.read.regret.absolute)

  interval <- stopTime / numGraphs;
  df <- data.frame(time=rep(0, numGraphs+1), regrets=rep(0, numGraphs+1))
  df$time = seq(0, numGraphs) * interval
  df$regrets = regrets

  if (ps) {
    des.postscript(paste(dir, "/../../../regret-average.eps", sep=""))
  }

  p <- ggplot(df, aes(x=time, y=regrets))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Average Total Regret")
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  numGraphs <- (graphs - 1);

  dir <- paste(sim, "/1/graphs", sep="")
  des.plot.incentive.deviate(dir, numGraphs, stopTime, TRUE)
  des.plot.regret.absolute(dir, numGraphs, stopTime, TRUE)
}


message("Finished.")
