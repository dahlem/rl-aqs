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
  ## (3) local control pattern, eta'
  m1 <- cbind(matrix(0, nrow=vertices, ncol=0))
  m2 <- cbind(matrix(0, nrow=vertices, ncol=0))
  for (i in 1:(graphs-1)) {
    graphFile <- paste(sim, "/1/graphs/graph", i, ".gml", sep="")
    print(paste("Read graph ", graphFile))
    graph <- read.graph(graphFile, format="graphml")

    ## 1.1 read specified property
    prop1 <- V(graph)$average_delay_in_queue
    prop2 <- V(graph)$regret_absolute
    df1 <- des.control.ownership(graph=graph, policy="wpl", nonTopProp=prop1)
    df2 <- des.control.ownership(graph=graph, policy="wpl", nonTopProp=prop2)

    ## sort according to id and correlate according to cint/phiint
    df1 <- df1[order(df1$id, decreasing=F),]
    df2 <- df2[order(df2$id, decreasing=F),]
    m1 <- cbind(m1, matrix(df1$phiint, nrow=vertices))
    m2 <- cbind(m2, matrix(df2$phiint, nrow=vertices))
  }

  delayKendall <- des.control.evo.corr(m1, method="kendall")
  df <- data.frame(time=seq(0,graphs-2) * interval, y=delayKendall)
  write.csv(df, "control-delay-kendall-correlation.dat")

  delayPearson <- des.control.evo.corr(m1, method="pearson")
  df <- data.frame(time=seq(0,graphs-2) * interval, y=delayPearson)
  write.csv(df, "control-delay-pearson-correlation.dat")

  regretKendall <- des.control.evo.corr(m2, method="kendall")
  df <- data.frame(time=seq(0,graphs-2) * interval, y=regretKendall)
  write.csv(df, "control-regret-kendall-correlation.dat")

  regretPearson <- des.control.evo.corr(m2, method="pearson")
  df <- data.frame(time=seq(0,graphs-2) * interval, y=regretPearson)
  write.csv(df, "control-regret-pearson-correlation.dat")
}


message("Finished.")
