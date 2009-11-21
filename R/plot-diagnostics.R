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
## Keywords: diagnostics
## Created: 15.04.2009


library(igraph)
library(ggplot2)


des.diagnostic.sum <- function(g, simNum, replications) {
    dflocal <- data.frame(eventInSystemTimes=rep(0, replications),
                          averageDelayInQueue=rep(0, replications),
                          utilisation=rep(0, replications));

    for (r in seq(1, replications)) {
      print(paste("Read graph: ", simNum, "/", r, "/graphs/graph", g, ".gml", sep=""))
      graphFile = paste(simNum, "/", r, "/graphs/graph", g, ".gml", sep="")
      graph = read.graph(graphFile, format="graphml")
      eventInSystemTimes <- get.vertex.attribute(graph, "avg_event_in_system_time");
      averageDelayInQueue <- get.vertex.attribute(graph, "average_delay_in_queue");
      utilisation <- get.vertex.attribute(graph, "utilisation");
      dflocal$eventInSystemTimes[r] <- sum(eventInSystemTimes)
      dflocal$averageDelayInQueue[r] <- sum(averageDelayInQueue)
      dflocal$utilisation[r] <- mean(utilisation)
    }

    ## calc mean and standard error
    return(list(g=g,
                meanResponse=mean(dflocal$eventInSystemTimes),
                seResponse=(sd(dflocal$eventInSystemTimes) / replications),
                meanDelay=mean(dflocal$averageDelayInQueue),
                seDelay=(sd(dflocal$averageDelayInQueue) / replications),
                meanUtilisation=mean(dflocal$utilisation),
                seUtilisation=(sd(dflocal$utilisation) / replications)
                ))
}


des.diagnostic.plots <- function(cl, simNum, replications, graphs, stopTime, ps=TRUE) {
  interval <- stopTime / (graphs - 1);
  graphs <- graphs - 1
  df <- data.frame(time=rep(0, graphs), meanResponse=rep(0, graphs), uclResponse=rep(0, graphs), lclResponse=rep(0, graphs),
                   meanDelay=rep(0, graphs), uclDelay=rep(0, graphs), lclDelay=rep(0, graphs),
                   meanUtilisation=rep(0, graphs), uclUtil=rep(0, graphs), lclUtil=rep(0, graphs));

  if (is.null(cl)) {
    result <- sapply(seq(1, graphs),
                     des.diagnostic.sum,
                     simNum=simNum,
                     replications=replications)
  } else {
    # execute in a cluster clusterApplyLB
    result <- parSapply(cl, seq(1, graphs),
                        des.diagnostic.sum,
                        simNum=simNum,
                        replications=replications)
  }

  write.table(t(result), paste(simNum, "/diagnostics.dat", sep=""), sep=",", row.names=FALSE, quote=FALSE)
  diagnostics <- read.csv(paste(simNum, "/diagnostics.dat", sep=""))
  
  sortedResult <- diagnostics[order(unlist(diagnostics[,1])), ]
  df$time = seq(1, graphs) * interval;
  df$meanResponse = sortedResult[,2]
  df$uclResponse = df$meanResponse + 1.96 * sortedResult[,3]
  df$lclResponse = df$meanResponse - 1.96 * sortedResult[,3]
  df$meanDelay = sortedResult[,4]
  df$uclDelay = df$meanDelay + 1.96 * sortedResult[,5]
  df$lclDelay = df$meanDelay - 1.96 * sortedResult[,5]
  df$meanUtilisation = sortedResult[,6]
  df$uclUtil = df$meanUtilisation + 1.96 * sortedResult[,7]
  df$lclUtil = df$meanUtilisation - 1.96 * sortedResult[,7]

  if (ps) {
    pdf(paste(simNum, "/graph-response-diagnostic-evo-plot.pdf", sep=""), onefile=FALSE, version=1.4)
  }


  p <- ggplot(df, aes(x=time, y=meanResponse))
  p <- p + layer(geom = "line")
  p <- p + geom_smooth(aes(min=lclResponse, max=uclResponse), stat="identity")
  p <- p + scale_y_continuous("Mean Total Response Time")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean Total Response Time")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }

  if (ps) {
    pdf(paste(simNum, "/graph-delay-diagnostic-evo-plot.pdf", sep=""), onefile=FALSE, version=1.4)
  }

  p <- ggplot(df, aes(x=time, y=meanDelay))
  p <- p + layer(geom = "line")
  p <- p + geom_smooth(aes(min=lclDelay, max=uclDelay), stat="identity")
  p <- p + scale_y_continuous("Mean Total Waiting Time")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean Total Waiting Time")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }

  if (ps) {
    pdf(paste(simNum, "/graph-util-diagnostic-evo-plot.pdf", sep=""), onefile=FALSE, version=1.4)
  }

  p <- ggplot(df, aes(x=time, y=meanUtilisation))
  p <- p + layer(geom = "line")
  p <- p + geom_smooth(aes(min=lclUtil, max=uclUtil), stat="identity")
  p <- p + scale_y_continuous("Mean Utilisation")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

cl <- NULL

# if it was configured using MPI, load the snow library
if (FALSE) {
  library(snow)
}

message("Start with the diagnostics!")


if (FALSE) {
  # read the NODEFILE to find out how many nodes are participating in the cluster
  nodes <- read.table("NODEFILE")
  
  # make the cluster
  cl <- makeCluster(length(nodes$V1), type="MPI")

  clusterEvalQ(cl, library(igraph))
  clusterEvalQ(cl, library(ggplot2))
}

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)

## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  reps <- simulations[simulations$sim_num == sim,]$actual_reps
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time

  ## (2) print the 
  des.diagnostic.plots(cl, sim, reps, graphs, stopTime, ps=TRUE)
}


# In case of unexpected program exceptions, clean up the cluster
.Last <- function()
{
  if (FALSE) {
    stopCluster(cl)
  }
}

message("Finished diagnostics.")
