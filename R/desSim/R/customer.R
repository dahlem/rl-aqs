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
## Keywords: analysis, discrete-event simulation, events, delay, path size
## Created: 25.10.2008

welford.mean <- function (n, x_bar, x) {
  d <- x - x_bar;
  x_bar_new <- x_bar + d / n;

  return(x_bar_new)
}

des.customer.read <- function(filename) {
  return(read.csv(filename));
}


des.plot.hist.customer.delay <- function(events, ps=TRUE) {
  if (ps) {
    postscript("mean-customer-delay.eps", onefile=FALSE)
  }

  finishedEvents <- events[events$type == 5,];
  df <- data.frame(delay = finishedEvents$delay);
  p <- ggplot(df, aes(x=delay)) 
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Delay")
  p <- p + opts(title="Histogram of Delay of completed Events")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}

des.plot.hist.node.emanating.delay <- function(events, ps=TRUE) {
  if (ps) {
    postscript("mean-node-emanating-event-delay.eps", onefile=FALSE)
  }

  ## select the nodes and remove -1
  df <- des.customer.meanDelay(events);

  p <- ggplot(df, aes(x=meanDelay)) 
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Mean Delay")
  p <- p + opts(title="Histogram of the Mean Delay of Events emanating from each Node")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.plot.hist.node.emanating.pathLength <- function(events, ps=TRUE) {
  if (ps) {
    postscript("mean-node-emanating-event-pathlength.eps", onefile=FALSE)
  }

  ## select the nodes and remove -1
  df <- des.customer.meanPathLength(events)
  
  p <- ggplot(df, aes(x=meanPathLength)) 
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Mean Path Length")
  p <- p + opts(title="Histogram of the Mean Path Length of Events emanating from each Node")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.plot.sys.mean.delay <- function(events, ps=TRUE) {
  if (ps) {
    postscript("mean-system-delay-evolution.eps", onefile=FALSE)
  }

  ## select the nodes and remove -1
  df <- des.system.meanDelay(events)
  
  p <- ggplot(df, aes(x=time, y=meanDelay))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Mean Delay")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean Delay in the System")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.plot.sys.mean.delay <- function(events, ps=TRUE) {
  if (ps) {
    postscript("mean-system-pathLength-evolution.eps", onefile=FALSE)
  }

  ## select the nodes and remove -1
  df <- des.system.pathLength(events)
  
  p <- ggplot(df, aes(x=time, y=pathLength))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Mean Path Length")
  p <- p + scale_x_continuous("Time")
  p <- p + opts(title="Evolution of Mean Path Length in the System")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.customer.meanDelay <- function(events) {
  nodes <- unique(events$destination)
  nodes <- nodes[which(nodes!=-1)];
  df <- data.frame(node=rep(0,length(nodes)),meanDelay=rep(0,length(nodes)));
  df$node <- nodes;
  numEventsCompleted <- 0;
  
  for(i in seq(1:length(df$node))) {
    emanatingEvents <- events[(events$origin == -1) & (events$destination == df$node[i]),];

    for (j in seq(1:length(emanatingEvents$id))) {
      completedEvent <- events[(events$id == emanatingEvents$id[j]) & (events$type == 5),];

      if (length(completedEvent$delay) != 0) {
        numEventsCompleted <- numEventsCompleted + 1;
        df$meanDelay[i] <- welford.mean(numEventsCompleted,
                                        df$meanDelay[i],
                                        completedEvent$delay);
      }
    }
    numEventsCompleted <- 0;
  }

  return(df)
}


des.system.meanDelay <- function(events) {
  completedEvents <- events[(events$type == 5), c(3, 4)];
  df <- data.frame(time=rep(0,length(completedEvents$delay)),
                   meanDelay=rep(0,length(completedEvents$delay)));

  df$time = completedEvents$arrivalTime;
  df$meanDelay[1] <- completedEvent$delay[1];
  
  for (i in seq(2, length(completedEvents$delay))) {
    df$meanDelay[i] <- welford.mean(i,
                                    df$meanDelay[i-1],
                                    completedEvents$delay[i]);
  }

  return(df)
}


des.customer.meanPathLength <- function(events) {
  nodes <- unique(events$destination)
  nodes <- nodes[which(nodes!=-1)];
  df <- data.frame(node=rep(0,length(nodes)),meanPathLength=rep(0,length(nodes)));
  df$node <- nodes;
  numEventsCompleted <- 0;
  
  for(i in seq(1:length(df$node))) {
    emanatingEvents <- events[(events$origin == -1) & (events$destination == df$node[i]),];

    for (event in emanatingEvents$id) {
      completedEvent <- events[(events$id == event),];

      if (length(completedEvent$eventPathSize) != 0) {
        numEventsCompleted <- numEventsCompleted + 1;
        df$meanPathLength[i] <- welford.mean(numEventsCompleted,
                                             df$meanPathLength[i],
                                             max(completedEvent$eventPathSize));
      }
    }
    numEventsCompleted <- 0;
  }

  return(df)
}

des.system.pathLength <- function(events) {
  completedEvents <- events[(events$type == 5), ];
  df <- data.frame(time=rep(0,length(completedEvents$eventPathSize)),
                   pathLength=rep(0,length(completedEvents$eventPathSize)));

  df$time = completedEvents$arrivalTime;

  completedEvent <- events[(events$id == completedEvents$id[1]),];
  df$pathLength[1] <- max(completedEvent$eventPathSize)
  
  for (i in seq(2, length(completedEvents$delay))) {
    completedEvent <- events[(events$id == completedEvents$id[i]),];
    df$pathLength[i] <- welford.mean(i,
                                     df$pathLength[i-1],
                                     max(completedEvent$eventPathSize));
  }

  return(df)
}


des.customer.betweenness.delay <- function(graphFile, meanDelay) {
  graph <- read.graph(graphFile, format="graphml");
  between <- betweenness(graph, directed = TRUE);
  dfSorted <- meanDelay[sort.list(meanDelay$node),];
  cor.test(dfSorted$meanDelay, between[1:49])
}


des.customer.betweenness.meanPathLength <- function(graphFile, meanPathLength) {
  graph <- read.graph(graphFile, format="graphml");
  between <- betweenness(graph, directed = TRUE);
  dfSorted <- meanPathLength[sort.list(meanPathLength$node),];
  cor.test(dfSorted$meanPathLength, between[1:49])
}
