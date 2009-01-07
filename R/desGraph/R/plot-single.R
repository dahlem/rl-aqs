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
## Keywords: plotting
## Created: 07.01.2009

des.graph.single.utilisation.evo.plot <- function(graphs, nodes, stopTime, ps=TRUE) {
  if (ps) {
    postscript("graph-single-utilisation-evo-plot.eps", onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1)
  graphs <- graphs - 1
  df <- data.frame(time=rep(0, graphs), utilisation=rep(0, graphs))

  for (i in seq(1, graphs)) {
    if (file.exists(paste("graphs/graph", i, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: graphs/graph", i, ".gml", sep=""))
      simEndGraph = paste("graphs/graph", i, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      utilisations <- get.vertex.attribute(graph, "utilisation");
      df$utilisation[i] <- mean(utilisations)
    }
  }

  df$time = seq(1, graphs) * interval;

  p <- ggplot(df, aes(x=time, y=utilisation))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Mean Utilisation")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean System Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.single.delay.evo.plot <- function(graphs, nodes, stopTime, ps=TRUE) {
  if (ps) {
    postscript("graph-single-delay-evo-plot.eps", onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1)
  graphs <- graphs - 1
  df <- data.frame(time=rep(0, graphs), delay=rep(0, graphs))

  for (i in seq(1, graphs)) {
    if (file.exists(paste("graphs/graph", i, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: graphs/graph", i, ".gml", sep=""))
      simEndGraph = paste("graphs/graph", i, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      delay <- get.vertex.attribute(graph, "average_delay_in_queue")
      df$delay[i] <- mean(delay)
    }
  }

  df$time = seq(1, graphs) * interval;

  p <- ggplot(df, aes(x=time, y=delay))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Mean Delay in Queue")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean Delay")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


library(igraph)
library(ggplot2)

replicas <- read.csv("../replica_results.dat", header=TRUE)
simNum <- replicas$sim_num[1]

simulations <- read.csv("../../simulations.dat", header=TRUE)

print(simulations)

graphs <- simulations[simulations$sim_num == simNum,]$graphs
nodes <- simulations[simulations$sim_num == simNum,]$network_size
stopTime <- simulations[simulations$sim_num == simNum,]$stop_time

des.graph.single.utilisation.evo.plot(graphs, nodes, stopTime)
des.graph.single.delay.evo.plot(graphs, nodes, stopTime)
