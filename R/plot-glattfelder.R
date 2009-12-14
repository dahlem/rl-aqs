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


des.backbone <- function(graph, vartheta, prop) {
  dat <- des.control.ownership(graph, policy="wpl", nonTopProp=prop)
  backbone <- des.backbone.control(dat, vartheta)
  etaPrime <- des.backbone.control.eta.prime(dat, vartheta)
  localControl <- des.backbone.control.local(graph, backbone, inout=1)
  return(data.frame(etaPrime, localControl))
}

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
  prop1 <- V(graph1)$average_delay_in_queue
  prop2 <- V(graph1)$incentive_deviate
  prop3 <- V(graph1)$regret_absolute
  df1 <- des.control.ownership(graph1, policy="wpl", prop1)
  df2 <- des.control.ownership(graph1, policy="wpl", prop2)
  df3 <- des.control.ownership(graph1, policy="wpl", prop3)
  write.csv(df1, "start-avg-delay-control.dat")
  write.csv(df2, "start-incentive-deviate-control.dat")
  write.csv(df3, "start-regret-absolute-control.dat")
  des.control.plot("start-avg-delay-", df1)
  des.control.plot("start-incentive-deviate-", df2)
  des.control.plot("start-regret-absolute-", df2)
  
  graph2 <- read.graph(paste(sim, "/1/graphs/graph", graphs-1, ".gml", sep=""), format="graphml")
  prop1 <- V(graph2)$average_delay_in_queue
  prop2 <- V(graph2)$incentive_deviate
  prop3 <- V(graph2)$regret_absolute
  df1 <- des.control.ownership(graph2, policy="wpl", prop1)
  df2 <- des.control.ownership(graph2, policy="wpl", prop2)
  df3 <- des.control.ownership(graph2, policy="wpl", prop3)
  write.csv(df1, "end-avg-delay-control.dat")
  write.csv(df2, "end-incentive-deviate-control.dat")
  write.csv(df3, "end-regret-absolute-control.dat")
  des.control.plot("end-avg-delay-", df1)
  des.control.plot("end-incentive-deviate-", df2)
  des.control.plot("end-regret-absolute-", df3)

  ## (3) local control pattern, eta'
  df1 <- data.frame(time=c(), localControl=c(), etaPrime=c())
  df2 <- data.frame()
  df3 <- data.frame()
  for (i in 1:(graphs-1)) {
    graphFile <- paste(dir, "/1/1/graphs/graph", i, ".gml", sep="")
    print(paste("Read graph ", graphFile))
    graph <- read.graph(graphFile, format="graphml")

    ## 1.1 read specified property
    prop1 <- V(graph)$average_delay_in_queue
    data1 <- des.backbone(graph, vartheta, prop1)
    df1 <- rbind(df1, data.frame(time=interval * 1,
                                 localControl=data1$localControl,
                                 etaPrime=data1$etaPrime))
    
    prop2 <- V(graph)$incentive_deviate
    data2 <- des.backbone(graph, vartheta, prop2)
    df2 <- rbind(df2, data.frame(time=interval * i,
                                 localControl=data2$localControl,
                                 etaPrime=data2$etaPrime))

    prop3 <- V(graph)$regret_absolute
    data3 <- des.backbone(graph, vartheta, prop3)
    df3 <- rbind(df3, data.frame(time=interval * i,
                                 localControl=data3$localControl,
                                 etaPrime=data3$etaPrime))
  }
  write.csv(df1, "evo-avg-delay-control.dat")
  write.csv(df2, "evo-incentive-deviate-control.dat")
  write.csv(df3, "evo-regret-absolute-control.dat")
}


message("Finished.")
