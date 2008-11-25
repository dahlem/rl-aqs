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

des.graph.service.plot <- function(graph, filename, ps=TRUE) {
  if (ps) {
    postscript(eval.dir(filename), onefile=FALSE)
  }

  plot(graph,
       vertex.label=get.vertex.attribute(graph, "service_rate"),
       edge.label=get.edge.attribute(graph, "weight"),
       vertex.label.family="Helvetica", edge.label.family="Helvetica")

  if (ps) {
    dev.off()
  }
}

des.graph.arrival.plot <- function(graph, filename, ps=TRUE) {
  if (ps) {
    postscript(eval.dir(filename), onefile=FALSE)
  }

  plot(graph,
       vertex.label=get.vertex.attribute(graph, "arrival_rate"),
       edge.label=get.edge.attribute(graph, "weight"),
       vertex.label.family="Helvetica", edge.label.family="Helvetica")

  if (ps) {
    dev.off()
  }
}

des.graph.plots <- function(simEndGraph, graphs, prefix, stopTime) {
  graph = read.graph(simEndGraph, format="graphml")
  
  des.graph.arrival.hist.plot(graph)
  des.graph.utilisation.plot(graph)
  des.graph.utilisation.evo.plot(prefix, graphs, stopTime)
  des.graph.average.delay.in.queue.plot(graph)
  des.graph.average.delay.in.queue.evo.plot(prefix, graphs, stopTime)
}

des.graph.arrival.hist.plot <- function(graph, ps=TRUE) {
  if (ps) {
    postscript("graph-arrival-hist-plot.eps", onefile=FALSE)
  }

  arrivalT <- get.vertex.attribute(graph, "arrival_rate");
  meanT <- mean(arrivalT)
  df <- data.frame(arrivalTimes=arrivalT)
  
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


des.graph.utilisation.plot <- function(graph, ps=TRUE) {
  if (ps) {
    postscript("graph-utilisation-hist-plot.eps", onefile=FALSE)
  }

  utilisations <- get.vertex.attribute(graph, "utilisation");
  meanU <- mean(utilisations)
  df <- data.frame(utilisation=utilisations)
  
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


des.graph.utilisation.evo.plot <- function(prefix, graphs, stopTime, ps=TRUE) {
  if (ps) {
    postscript("graph-utilisation-evo-plot.eps", onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1);
  df <- data.frame(time=rep(0, graphs), meanUtil=rep(0, graphs));

  df$time = seq(0, (graphs - 1)) * interval;
  
  for (i in seq(0, graphs - 1)) {
    graph <- read.graph(paste(prefix, "/graph", i, ".gml", sep=""), format="graphml")
    utilisations <- get.vertex.attribute(graph, "utilisation");
    df$meanUtil[i + 1] <- mean(utilisations)
  }
  
  p <- ggplot(df, aes(x=time, y=meanUtil))
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


des.graph.average.delay.in.queue.plot <- function(graph, ps=TRUE) {
  if (ps) {
    postscript("graph-average-delay-in-queue-hist-plot.eps", onefile=FALSE)
  }

  averageDelayInQueue <- get.vertex.attribute(graph, "average_delay_in_queue");
  meanD <- mean(averageDelayInQueue)
  df <- data.frame(averageDelay=averageDelayInQueue)
  
  p <- ggplot(df, aes(x=averageDelay)) 
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


des.graph.average.delay.in.queue.evo.plot <- function(prefix, graphs, stopTime, ps=TRUE) {
  if (ps) {
    postscript("graph-average-delay-in-queue-evo-plot.eps", onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1);
  df <- data.frame(time=rep(0, graphs), averageDelay=rep(0, graphs));

  df$time = seq(0, (graphs - 1)) * interval;
  
  for (i in seq(0, graphs - 1)) {
    graph <- read.graph(paste(prefix, "/graph", i, ".gml", sep=""), format="graphml")
    averageDelay <- get.vertex.attribute(graph, "average_delay_in_queue");
    df$averageDelay[i + 1] <- mean(averageDelay)
  }
  
  p <- ggplot(df, aes(x=time, y=averageDelay))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Average Delay")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Average Delay in Queue in the System")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}
