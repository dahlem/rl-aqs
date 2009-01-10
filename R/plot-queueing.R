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
## Created: 10.01.2009

des.queueing.main <- function(qts, graphs) {
  des.queueing.arrival.hist.plot(qts)
  des.queueing.arrival.power.law.plot(qts)
  des.queueing.average.response.hist.plot(qts)
  des.queueing.average.response.power.law.plot(qts)
  des.queueing.degree.hist.plot(qts, graphs)
  des.queueing.degree.power.law.plot(qts, graphs)
  des.queueing.service.power.law.plot(qts)
  des.queueing.utilisation.hist.plot(qts)
  des.queueing.numEvents.betweenness.plot(qts, graphs)
  des.queueing.utilisation.betweenness.plot(qts, graphs)
  des.queueing.serviceRate.betweenness.plot(qts, graphs)
  des.queueing.delay.betweenness.plot(qts, graphs)
  des.queueing.events.betweenness.plot(qts, graphs)
  des.queueing.numEvents.degree.plot(qts, graphs)
  des.queueing.utilisation.degree.plot(qts, graphs)
  des.queueing.delay.degree.plot(qts, graphs)
  des.queueing.delay.utilisation.plot(qts, graphs)
}

des.queueing.arrival.hist.plot <- function(qts, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-arrival-hist-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$lambda)
  df <- data.frame(lambda=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    df$lambda[(1 + (i - 1) * nodes):(nodes * i)] = qts[[i]]$lambda
  }
  
  p <- ggplot(df, aes(x=lambda))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Total Arrival Rates")
  p <- p + opts(title="Histogram of total Vertex Poisson Arrival Rates")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.arrival.power.law.plot <- function(qts, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-arrival-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$lambda)
  df <- data.frame(lambda=rep(0, length(qts) * nodes),
                   rank=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    lambda <- qts[[i]]$lambda
    rank <- rank(lambda)
    df$lambda[(1 + (i - 1) * nodes):(nodes * i)] = lambda
    df$rank[(1 + (i - 1) * nodes):(nodes * i)] = rank
  }

  p <- ggplot(df, aes(x=rank,y=lambda))
  p <- p + layer(geom = "point")
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Total Arrival Rates")
  p <- p + scale_x_continuous("Rank")
  p <- p + opts(title="Total Vertex Poisson Arrival Rates by Rank")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.degree.hist.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-hist-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$lambda)
  df <- data.frame(degree=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    df$degree[(1 + (i - 1) * nodes):(nodes * i)] = degree(graphs[[i]], mode="in")
  }

  p <- ggplot(df, aes(x=degree))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("In-Degree")
  p <- p + opts(title="Histogram of the Vertex In-Degree")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.degree.power.law.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$lambda)
  df <- data.frame(degree=rep(0, length(qts) * nodes),
                   rank=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    degree <- degree(graphs[[i]], mode="in")
    rank <- rank(degree)
    df$degree[(1 + (i - 1) * nodes):(nodes * i)] = degree
    df$rank[(1 + (i - 1) * nodes):(nodes * i)] = rank
  }

  df <- df[df$degree > 0,]
  
  p <- ggplot(df, aes(x=rank,y=degree))
  p <- p + layer(geom = "point")
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Vertex In-Degree")
  p <- p + scale_x_continuous("Rank")
  p <- p + opts(title="Vertex In-Degree by Rank")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.average.response.hist.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-average-response-hist-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$w)
  df <- data.frame(response=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    df$response[(1 + (i - 1) * nodes):(nodes * i)] = qts[[i]]$w
  }

  p <- ggplot(df, aes(x=response))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Average Response Time")
  p <- p + opts(title="Histogram of the Average Response Times")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.average.response.power.law.plot <- function(qts, ps=TRUE) {
  if (ps) {
    postscript("queueing-average-response-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$w)
  df <- data.frame(response=rep(0, length(qts) * nodes),
                   rank=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    response <- qts[[i]]$w
    rank <- rank(response)
    df$response[(1 + (i - 1) * nodes):(nodes * i)] = response
    df$rank[(1 + (i - 1) * nodes):(nodes * i)] = rank
  }

  p <- ggplot(df, aes(x=rank,y=response))
  p <- p + layer(geom = "point")
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Average Response Time")
  p <- p + scale_x_continuous("Rank")
  p <- p + opts(title="Average Response Times by Rank")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.queueing.service.power.law.plot <- function(qts, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-service-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$mu)
  df <- data.frame(mu=rep(0, length(qts) * nodes),
                   rank=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    mu <- qts[[i]]$mu
    rank <- rank(mu)
    df$mu[(1 + (i - 1) * nodes):(nodes * i)] = mu
    df$rank[(1 + (i - 1) * nodes):(nodes * i)] = rank
  }

  p <- ggplot(df, aes(x=rank,y=mu))
  p <- p + layer(geom = "point")
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Service Rates")
  p <- p + scale_x_continuous("Rank")
  p <- p + opts(title="Exponential Rates by Rank")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.queueing.utilisation.hist.plot <- function(qts, ps=TRUE) {
  if (ps) {
    postscript("queueing-utilisation-hist-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$rho)
  df <- data.frame(rho=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    rho <- qts[[i]]$rho
    df$rho[(1 + (i - 1) * nodes):(nodes * i)] = rho
  }

  p <- ggplot(df, aes(x=rho))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=2, fill=NA, colour="black")
  p <- p + scale_y_continuous("")
  p <- p + scale_x_continuous("Utilisation")
  p <- p + opts(title="Histogram of Vertex Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.numEvents.betweenness.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-numEvents-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$L)
  df <- data.frame(betweenness=rep(0, length(qts) * nodes),
                   expectedAverageNumEvents=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    expectedAverageNumEvents <- as.vector(qts[[i]]$L)
    betweenness <- betweenness(graphs[[i]], directed=TRUE)
    df$betweenness[(1 + (i - 1) * nodes):(nodes * i)] = betweenness
    df$expectedAverageNumEvents[(1 + (i - 1) * nodes):(nodes * i)] = expectedAverageNumEvents
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


des.queueing.utilisation.betweenness.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-util-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$rho)
  df <- data.frame(betweenness=rep(0, length(qts) * nodes),
                   utilisation=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    utilisation <- as.vector(qts[[i]]$rho)
    betweenness <- betweenness(graphs[[i]], directed=TRUE)
    df$betweenness[(1 + (i - 1) * nodes):(nodes * i)] = betweenness
    df$utilisation[(1 + (i - 1) * nodes):(nodes * i)] = utilisation
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


des.queueing.serviceRate.betweenness.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-serviceRate-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$mu)
  df <- data.frame(betweenness=rep(0, length(qts) * nodes),
                   serviceRate=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    serviceRate <- as.vector(qts[[i]]$mu)
    betweenness <- betweenness(graphs[[i]], directed=TRUE)
    df$serviceRate[(1 + (i - 1) * nodes):(nodes * i)] = serviceRate
    df$betweenness[(1 + (i - 1) * nodes):(nodes * i)] = betweenness
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

des.queueing.delay.betweenness.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-delay-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$r)
  df <- data.frame(betweenness=rep(0, length(qts) * nodes),
                   delay=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    delay <- as.vector(qts[[i]]$r)
    betweenness <- betweenness(graphs[[i]], directed=TRUE)
    df$delay[(1 + (i - 1) * nodes):(nodes * i)] = delay
    df$betweenness[(1 + (i - 1) * nodes):(nodes * i)] = betweenness
  }

  ## select only vertices with betweenness > 0
  df <- df[df$betweenness > 0,]

  p <- ggplot(df, aes(y=delay, x=betweenness))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("Betweenness")
  p <- p + scale_y_continuous("Average Delay in Queue")
  p <- p + opts(title="Betweenness vs. Average Delay in Queue")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.queueing.events.betweenness.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-events.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$upsilon)
  df <- data.frame(betweenness=rep(0, length(qts) * nodes),
                   events=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    events <- as.vector(qts[[i]]$upsilon)
    betweenness <- betweenness(graphs[[i]], directed=TRUE)
    df$events[(1 + (i - 1) * nodes):(nodes * i)] = events
    df$betweenness[(1 + (i - 1) * nodes):(nodes * i)] = betweenness
  }

  ## select only vertices with betweenness > 0
  df <- df[df$betweenness > 0,]

  p <- ggplot(df, aes(y=events, x=betweenness))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("Betweenness")
  p <- p + scale_y_continuous("Average Number of Times a Job visits Node i")
  p <- p + opts(title="Betweenness vs. Average Number of Times a Job visits Node i")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.numEvents.degree.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-vs-numEvents-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$L)
  df <- data.frame(degree=rep(0, length(qts) * nodes),
                   expectedAverageNumEvents=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    expectedAverageNumEvents <- as.vector(qts[[i]]$L)
    degree <- degree(graphs[[i]], mode="in")
    df$expectedAverageNumEvents[(1 + (i - 1) * nodes):(nodes * i)] = expectedAverageNumEvents
    df$degree[(1 + (i - 1) * nodes):(nodes * i)] = degree
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


des.queueing.utilisation.degree.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-vs-util-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$rho)
  df <- data.frame(degree=rep(0, length(qts) * nodes),
                   utilisation=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    utilisation <- as.vector(qts[[i]]$rho)
    degree <- degree(graphs[[i]], mode="in")
    df$utilisation[(1 + (i - 1) * nodes):(nodes * i)] = utilisation
    df$degree[(1 + (i - 1) * nodes):(nodes * i)] = degree
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

des.queueing.delay.degree.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-vs-delay-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$r)
  df <- data.frame(degree=rep(0, length(qts) * nodes),
                   delay=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    delay <- as.vector(qts[[i]]$r)
    degree <- degree(graphs[[i]], mode="in")
    df$delay[(1 + (i - 1) * nodes):(nodes * i)] = delay
    df$degree[(1 + (i - 1) * nodes):(nodes * i)] = degree
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

des.queueing.delay.utilisation.plot <- function(qts, graphs, ps=TRUE) {
  if (ps) {
    postscript("queueing-delay-vs-utilisation-plot.eps", onefile=FALSE)
  }

  nodes <- length(qts[[1]]$r)
  df <- data.frame(utilisation=rep(0, length(qts) * nodes),
                   delay=rep(0, length(qts) * nodes))

  for (i in seq(1, length(qts))) {
    delay <- as.vector(qts[[i]]$r)
    utilisation <- as.vector(qts[[i]]$rho)
    df$delay[(1 + (i - 1) * nodes):(nodes * i)] = delay
    df$utilisation[(1 + (i - 1) * nodes):(nodes * i)] = utilisation
  }

  p <- ggplot(df, aes(x=utilisation, y=delay))
  p <- p + layer(geom = "point")
  p <- p + scale_y_continuous("Delay")
  p <- p + scale_x_continuous("Utilisation")
  p <- p + opts(title="Delay vs. Utilisation")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


library(desGraph)

dir <- dir()
filtered <- grep(".gml", dir)

qts <- list()
graphs <- list()

for (i in filtered) {
  print(paste("Read graph: ", dir[i]))
  graph <- read.graph(dir[i], format="graphml")
  qt <- des.queueing(graph)
  qts[[length(qts)+1]] = qt
  graphs[[length(graphs)+1]] = graph
}

des.queueing.main(qts, graphs)
