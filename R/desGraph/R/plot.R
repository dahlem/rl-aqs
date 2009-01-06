## Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

des.graph.sim.main <- function(replications, graphs, nodes, stopTime) {
  des.graph.arrival.hist.plot(replications, graphs, nodes)
  des.graph.utilisation.plot(replications, graphs, nodes)
  des.graph.utilisation.evo.plot(replications, graphs, nodes, stopTime)
  des.graph.utilisation.customers.evo.plot(replications, graphs, nodes, stopTime)
  des.graph.delay.customers.evo.plot(replications, graphs, nodes, stopTime)
  des.graph.average.delay.in.queue.plot(replications, graphs, nodes)
  des.graph.average.delay.in.queue.evo.plot(replications, graphs, nodes, stopTime)

  des.graph.numEvents.betweenness.plot(replications, graphs, nodes)
  des.graph.utilisation.betweenness.plot(replications, graphs, nodes)
  des.graph.delay.betweenness.plot(replications, graphs, nodes)
  des.graph.serviceRate.betweenness.plot(replications, graphs, nodes)
  
  des.graph.numEvents.degree.plot(replications, graphs, nodes)
  des.graph.utilisation.degree.plot(replications, graphs, nodes)
  des.graph.delay.degree.plot(replications, graphs, nodes)
}

des.graph.arrival.hist.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-arrival-hist-plot.eps", onefile=FALSE)
  }

  df <- data.frame(arrivalTimes=rep(0, nodes * replications));
  
  for (r in seq(1, replications)) {
    print(paste("Read graph: ", r, "/graphs/graph0.gml", sep=""))
    simEndGraph = paste(r, "/graphs/graph0.gml", sep="")
    graph = read.graph(simEndGraph, format="graphml")
    arrivalT <- get.vertex.attribute(graph, "arrival_rate");
    df$arrivalTimes[(1 + (r - 1) * nodes):(nodes * r)] = arrivalT
  }

  meanT <- mean(df$arrivalTimes)
  
  p <- ggplot(df, aes(x=arrivalTimes)) 
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + geom_vline(intercept=meanT, colour="red")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Arrival Rates")
  p <- p + opts(title="Histogram of Vertex Poisson Arrival Rates")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.utilisation.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-utilisation-hist-plot.eps", onefile=FALSE)
  }

  files <- paste(seq(1, replications), "/graphs/graph", graphs - 1, ".gml", sep="")
  fexists <- file.exists(files)
  ftrue <- fexists[fexists == TRUE]
  df <- data.frame(utilisation=rep(0, nodes * length(ftrue)));
  counter <- 1
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      utilisations <- get.vertex.attribute(graph, "utilisation");
      df$utilisation[(1 + (counter - 1) * nodes):(nodes * counter)] <- utilisations
      counter <- counter + 1
    }
  }

  meanU <- mean(df$utilisation)
  
  p <- ggplot(df, aes(x=utilisation)) 
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + geom_vline(intercept=meanU, colour="red")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Utilisation")
  p <- p + opts(title="Histogram of Vertex Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.utilisation.evo.plot <- function(replications, graphs, nodes, stopTime, ps=TRUE) {
  if (ps) {
    pdf("graph-utilisation-evo-plot.pdf", onefile=FALSE, version=1.4)
  }

  interval <- stopTime / (graphs - 1);
  graphs <- graphs - 1
  df <- data.frame(time=rep(0, graphs), meanUtil=rep(0, graphs), ucl=rep(0, graphs), lcl=rep(0, graphs));

  for (i in seq(1, graphs)) {
    files <- paste(seq(1, replications), "/graphs/graph", i, ".gml", sep="")
    fexists <- file.exists(files)
    ftrue <- fexists[fexists == TRUE]
    dflocal <- data.frame(utilisation=rep(0, nodes * length(ftrue)));
    counter <- 1
    
    for (r in seq(1, replications)) {
      if (file.exists(paste(r, "/graphs/graph", i, ".gml", sep="")) == TRUE) {
        print(paste("Read graph: ", r, "/graphs/graph", i, ".gml", sep=""))
        simEndGraph = paste(r, "/graphs/graph", i, ".gml", sep="")
        graph = read.graph(simEndGraph, format="graphml")
        utilisations <- get.vertex.attribute(graph, "utilisation");
        dflocal$utilisation[(1 + (counter - 1) * nodes):(nodes * counter)] <- utilisations
        counter <- counter + 1
      }
    }

    ## calc mean and standard error
    util.se <- sd(dflocal$utilisation) / sqrt(length(dflocal$utilisation))
    df$meanUtil[i] <- mean(dflocal$utilisation)
    df$ucl[i] <- df$meanUtil[i] + 1.96 * util.se
    df$lcl[i] <- df$meanUtil[i] - 1.96 * util.se
  }

  df$time = seq(1, graphs) * interval;
  
  p <- ggplot(df, aes(x=time, y=meanUtil))
  p <- p + layer(geom = "line")
  p <- p + geom_smooth(aes(min=lcl, max=ucl), stat="identity")
  p <- p + scale_y_continuous("Mean Utilisation")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean System Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.utilisation.customers.evo.plot <- function(replications, graphs, nodes, stopTime, ps=TRUE) {
  if (ps) {
    postscript("graph-utilisation-vs-customers-evo-plot.eps", onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1);
  df <- data.frame(time=rep(0, graphs), meanUtil=rep(0, graphs),
                   expectedAverageNumEvents=rep(0, graphs));

  for (i in seq(0, graphs - 1)) {
    files <- paste(seq(1, replications), "/graphs/graph", i, ".gml", sep="")
    fexists <- file.exists(files)
    ftrue <- fexists[fexists == TRUE]
    dflocal <- data.frame(utilisation=rep(0, nodes * length(ftrue)),
                          expectedAverageNumEvents=rep(0, nodes * length(ftrue)));
    counter <- 1
    
    for (r in seq(1, replications)) {
      if (file.exists(paste(r, "/graphs/graph", i, ".gml", sep="")) == TRUE) {
        print(paste("Read graph: ", r, "/graphs/graph", i, ".gml", sep=""))
        simEndGraph = paste(r, "/graphs/graph", i, ".gml", sep="")
        graph = read.graph(simEndGraph, format="graphml")
        utilisations <- get.vertex.attribute(graph, "utilisation")
        expectedAverageNumEvents <- get.vertex.attribute(graph, "expected_average_number_event")
        dflocal$utilisation[(1 + (counter - 1) * nodes):(nodes * counter)] <- utilisations
        dflocal$expectedAverageNumEvents[(1 + (counter - 1) * nodes):(nodes * counter)] <- expectedAverageNumEvents
        counter <- counter + 1
      }
    }

    ## calc mean and standard error
    util.se <- sd(dflocal$utilisation) / sqrt(length(dflocal$utilisation))
    df$meanUtil[i+1] <- mean(dflocal$utilisation)
    df$expectedAverageNumEvents[i+1] <- mean(dflocal$expectedAverageNumEvents)
  }

  df$time = seq(0, (graphs - 1)) * interval;

  p <- ggplot(df, aes(x=meanUtil, y=expectedAverageNumEvents))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Expected Average Number of Events in Queue")
  p <- p + scale_x_continuous("Mean Utilisation")
  p <- p + opts(title="Expected Average Number of Events vs. Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.numEvents.betweenness.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-betweenness-vs-numEvents-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(expectedAverageNumEvents=rep(0, replications * nodes),
                   betweenness=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      expectedAverageNumEvents <- get.vertex.attribute(graph, "expected_average_number_event")
      betweenness <- betweenness(graph, directed = TRUE);
      df$expectedAverageNumEvents[(1 + (counter - 1) * nodes):(nodes * counter)] <- expectedAverageNumEvents
      df$betweenness[(1 + (counter - 1) * nodes):(nodes * counter)] <- betweenness
      counter <- counter + 1
    }
  }

  ## select only vertices with betweenness > 0
  df <- df[df$betweenness > 0,]
  
  p <- ggplot(df, aes(y=expectedAverageNumEvents, x=betweenness))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("Betweenness")
  p <- p + scale_y_continuous("Expected Average Number of Events in Queue")
  p <- p + opts(title="Betweenness vs. Expected Average Number of Events in Queue")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.utilisation.betweenness.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-betweenness-vs-util-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(utilisation=rep(0, replications * nodes),
                   betweenness=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      utilisation <- get.vertex.attribute(graph, "utilisation")
      betweenness <- betweenness(graph, directed = TRUE);
      df$utilisation[(1 + (counter - 1) * nodes):(nodes * counter)] <- utilisation
      df$betweenness[(1 + (counter - 1) * nodes):(nodes * counter)] <- betweenness
      counter <- counter + 1
    }
  }

  ## select only vertices with betweenness > 0
  df <- df[df$betweenness > 0,]
  
  p <- ggplot(df, aes(y=utilisation, x=betweenness))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("Betweenness")
  p <- p + scale_y_continuous("Utilisation")
  p <- p + opts(title="Betweenness vs. Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.serviceRate.betweenness.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-betweenness-vs-serviceRate-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(serviceRate=rep(0, replications * nodes),
                   betweenness=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      serviceRate <- get.vertex.attribute(graph, "service_rate")
      betweenness <- betweenness(graph, directed = TRUE);
      df$serviceRate[(1 + (counter - 1) * nodes):(nodes * counter)] <- serviceRate
      df$betweenness[(1 + (counter - 1) * nodes):(nodes * counter)] <- betweenness
      counter <- counter + 1
    }
  }

  ## select only vertices with betweenness > 0
  df <- df[df$betweenness > 0,]
  
  p <- ggplot(df, aes(y=serviceRate, x=betweenness))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("Betweenness")
  p <- p + scale_y_continuous("Service Rate")
  p <- p + opts(title="Betweenness vs. Service Rate")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.delay.betweenness.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-betweenness-vs-delay-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(delay=rep(0, replications * nodes),
                   betweenness=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      delay <- get.vertex.attribute(graph, "average_delay_in_queue")
      betweenness <- betweenness(graph, directed = TRUE);
      df$delay[(1 + (counter - 1) * nodes):(nodes * counter)] <- delay
      df$betweenness[(1 + (counter - 1) * nodes):(nodes * counter)] <- betweenness
      counter <- counter + 1
    }
  }

  ## select only vertices with betweenness > 0
  df <- df[df$betweenness > 0,]
  
  p <- ggplot(df, aes(y=delay, x=betweenness))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("Betweenness")
  p <- p + scale_y_continuous("Average Delay in Queue")
  p <- p + opts(title="Average Delay in Queue vs. utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.numEvents.degree.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-degree-vs-numEvents-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(expectedAverageNumEvents=rep(0, replications * nodes),
                   degree=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      expectedAverageNumEvents <- get.vertex.attribute(graph, "expected_average_number_event")
      deg <- degree(graph, mode="in")
      df$expectedAverageNumEvents[(1 + (counter - 1) * nodes):(nodes * counter)] <- expectedAverageNumEvents
      df$degree[(1 + (counter - 1) * nodes):(nodes * counter)] <- deg
      counter <- counter + 1
    }
  }

  p <- ggplot(df, aes(y=expectedAverageNumEvents, x=degree))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("In-Degree")
  p <- p + scale_y_continuous("Expected Average Number of Events in Queue")
  p <- p + opts(title="In-Degree vs. Expected Average Number of Events in Queue")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.utilisation.degree.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-degree-vs-util-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(utilisation=rep(0, replications * nodes),
                   degree=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      utilisation <- get.vertex.attribute(graph, "utilisation")
      deg <- degree(graph, mode="in")
      df$utilisation[(1 + (counter - 1) * nodes):(nodes * counter)] <- utilisation
      df$degree[(1 + (counter - 1) * nodes):(nodes * counter)] <- deg
      counter <- counter + 1
    }
  }
  
  p <- ggplot(df, aes(y=utilisation, x=degree))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("In-Degree")
  p <- p + scale_y_continuous("Utilisation")
  p <- p + opts(title="In-Degree vs. Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.delay.degree.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-degree-vs-delay-plot.eps", onefile=FALSE)
  }

  counter <- 1
  df <- data.frame(delay=rep(0, replications * nodes),
                   degree=rep(0, replications * nodes));
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      delay <- get.vertex.attribute(graph, "average_delay_in_queue")
      deg <- degree(graph, mode="in")
      df$delay[(1 + (counter - 1) * nodes):(nodes * counter)] <- delay
      df$degree[(1 + (counter - 1) * nodes):(nodes * counter)] <- deg
      counter <- counter + 1
    }
  }
  
  p <- ggplot(df, aes(y=delay, x=degree))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("In-Degree")
  p <- p + scale_y_continuous("Average Delay in Queue")
  p <- p + opts(title="In-Degree vs. Average Delay in Queue")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.delay.customers.evo.plot <- function(replications, graphs, nodes, stopTime, ps=TRUE) {
  if (ps) {
    postscript("graph-delay-vs-customers-evo-plot.eps", onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1);
  df <- data.frame(time=rep(0, graphs), meanDelay=rep(0, graphs),
                   meanUtil=rep(0, graphs));

  for (i in seq(0, graphs - 1)) {
    files <- paste(seq(1, replications), "/graphs/graph", i, ".gml", sep="")
    fexists <- file.exists(files)
    ftrue <- fexists[fexists == TRUE]
    dflocal <- data.frame(utilisation=rep(0, nodes * length(ftrue)),
                          delay=rep(0, nodes * length(ftrue)));
    counter <- 1
    
    for (r in seq(1, replications)) {
      if (file.exists(paste(r, "/graphs/graph", i, ".gml", sep="")) == TRUE) {
        print(paste("Read graph: ", r, "/graphs/graph", i, ".gml", sep=""))
        simEndGraph = paste(r, "/graphs/graph", i, ".gml", sep="")
        graph = read.graph(simEndGraph, format="graphml")
        utilisations <- get.vertex.attribute(graph, "utilisation")
        delay <- get.vertex.attribute(graph, "average_delay_in_queue")
        dflocal$utilisation[(1 + (counter - 1) * nodes):(nodes * counter)] <- utilisations
        dflocal$delay[(1 + (counter - 1) * nodes):(nodes * counter)] <- delay
        counter <- counter + 1
      }
    }

    ## calc mean and standard error
    df$meanUtil[i+1] <- mean(dflocal$utilisation)
    df$meanDelay[i+1] <- mean(dflocal$delay)
  }

  df$time = seq(0, (graphs - 1)) * interval;

  p <- ggplot(df, aes(x=meanUtil, y=meanDelay))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Mean Delay")
  p <- p + scale_x_continuous("Mean Utilisation")
  p <- p + opts(title="Mean Delay vs. Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.average.delay.in.queue.plot <- function(replications, graphs, nodes, ps=TRUE) {
  if (ps) {
    postscript("graph-average-delay-in-queue-hist-plot.eps", onefile=FALSE)
  }

  files <- paste(seq(1, replications), "/graphs/graph", graphs - 1, ".gml", sep="")
  fexists <- file.exists(files)
  ftrue <- fexists[fexists == TRUE]
  df <- data.frame(averageDelayInQueue=rep(0, nodes * length(ftrue)));
  counter <- 1
  
  for (r in seq(1, replications)) {
    if (file.exists(paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")) == TRUE) {
      print(paste("Read graph: ", r, "/graphs/graph", graphs - 1, ".gml", sep=""))
      simEndGraph = paste(r, "/graphs/graph", graphs - 1, ".gml", sep="")
      graph = read.graph(simEndGraph, format="graphml")
      averageDelayInQueue <- get.vertex.attribute(graph, "average_delay_in_queue");
      df$averageDelayInQueue[(1 + (counter - 1) * nodes):(nodes * counter)] <- averageDelayInQueue
      counter <- counter + 1
    }
  }

  meanD <- mean(df$averageDelayInQueue)

  p <- ggplot(df, aes(x=averageDelayInQueue)) 
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + geom_vline(intercept=meanD, colour="red")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Average Delay in Queue")
  p <- p + opts(title="Histogram of Vertex Average Delay in Queue")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.graph.average.delay.in.queue.evo.plot <- function(replications, graphs, nodes, stopTime, ps=TRUE) {
  if (ps) {
    pdf("graph-average-delay-in-queue-evo-plot.pdf", onefile=FALSE, version=1.4)
  }

  interval <- stopTime / (graphs - 1);
  graphs <- graphs - 1
  df <- data.frame(time=rep(0, graphs), meanDelay=rep(0, graphs), ucl=rep(0, graphs), lcl=rep(0, graphs));

  for (i in seq(1, graphs)) {
    files <- paste(seq(1, replications), "/graphs/graph", i, ".gml", sep="")
    fexists <- file.exists(files)
    ftrue <- fexists[fexists == TRUE]
    dflocal <- data.frame(delay=rep(0, nodes * length(ftrue)));
    counter <- 1
    
    for (r in seq(1, replications)) {
      if (file.exists(paste(r, "/graphs/graph", i, ".gml", sep="")) == TRUE) {
        print(paste("Read graph: ", r, "/graphs/graph", i, ".gml", sep=""))
        simEndGraph = paste(r, "/graphs/graph", i, ".gml", sep="")
        graph = read.graph(simEndGraph, format="graphml")
        delay <- get.vertex.attribute(graph, "average_delay_in_queue");
        dflocal$delay[(1 + (counter - 1) * nodes):(nodes * counter)] <- delay
        counter <- counter + 1
      }
    }

    ## calc mean and standard error
    delay.se <- sd(dflocal$delay) / sqrt(length(dflocal$delay))
    df$meanDelay[i] <- mean(dflocal$delay)
    df$ucl[i] <- df$meanDelay[i] + 1.96 * delay.se
    df$lcl[i] <- df$meanDelay[i] - 1.96 * delay.se
  }

  df$time = seq(1, graphs) * interval;

  p <- ggplot(df, aes(x=time, y=meanDelay))
  p <- p + layer(geom = "line")
  p <- p + geom_smooth(aes(min=lcl, max=ucl), stat="identity")
  p <- p + scale_y_continuous("Average Delay")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Average Delay in Queue in the System")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


library(igraph)
library(ggplot2)

replicas <- read.csv("replica_results.dat", header=TRUE)
simNum <- replicas$sim_num[1]

simulations <- read.csv("../simulations.dat", header=TRUE)
replications <- simulations[simulations$sim_num == simNum,]$actual_reps
graphs <- simulations[simulations$sim_num == simNum,]$graphs
nodes <- simulations[simulations$sim_num == simNum,]$network_size
stopTime <- simulations[simulations$sim_num == simNum,]$stop_time

des.graph.sim.main(replications, graphs, nodes, stopTime)
