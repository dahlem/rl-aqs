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

des.queueing.arrival.hist.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-arrival-hist-plot.eps", onefile=FALSE)
  }

  df <- data.frame(lambda=as.vector(qt$lambda))
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

des.queueing.arrival.power.law.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-arrival-power-law-plot.eps", onefile=FALSE)
  }

  lambda <- as.vector(qt$lambda)
  rank <- rank(lambda)
  df <- data.frame(lambda=lambda, rank=rank)
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

des.queueing.degree.hist.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-hist-plot.eps", onefile=FALSE)
  }

  degree <- degree(graph, mode="in")
  df <- data.frame(degree);

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

des.queueing.degree.power.law.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-power-law-plot.eps", onefile=FALSE)
  }

  degree <- degree(graph, mode="in")
  rank <- rank(lambda)
  df <- data.frame(degree, rank)
  p <- ggplot(df, aes(x=rank,y=lambda))
  p <- p + layer(geom = "point")
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("In-Degree")
  p <- p + scale_x_continuous("Rank")
  p <- p + opts(title="In-Degree by Rank")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.queueing.average.response.hist.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-average-response-hist-plot.eps", onefile=FALSE)
  }

  response <- as.vector(qt$w)
  df <- data.frame(response);

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

des.queueing.average.response.power.law.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-average-response-power-law-plot.eps", onefile=FALSE)
  }

  response <- as.vector(qt$w)
  rank <- rank(lambda)
  df <- data.frame(response, rank)
  p <- ggplot(df, aes(x=rank,y=lambda))
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


des.queueing.service.power.law.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-service-power-law-plot.eps", onefile=FALSE)
  }

  mu <- as.vector(qt$mu)
  rank <- rank(lambda)
  df <- data.frame(mu, rank)
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


des.queueing.utilisation.plot <- function(qt, ps=TRUE) {
  if (ps) {
    postscript("queueing-utilisation-hist-plot.eps", onefile=FALSE)
  }

  df <- data.frame(utilisation=as.vector(qt$rho))
  p <- ggplot(df, aes(x=utilisation))
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


des.queueing.numEvents.betweenness.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-numEvents-plot.eps", onefile=FALSE)
  }

  expectedAverageNumEvents <- as.vector(qt$L)
  betweenness <- betweenness(graph, directed=TRUE)
  df <- data.frame(expectedAverageNumEvents,
                   betweenness);

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


des.queueing.utilisation.betweenness.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-util-plot.eps", onefile=FALSE)
  }

  utilisation <- as.vector(qt$rho)
  betweenness <- betweenness(graph, directed=TRUE)
  df <- data.frame(utilisation, betweenness);

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


des.queueing.serviceRate.betweenness.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-serviceRate-plot.eps", onefile=FALSE)
  }

  serviceRate <- as.vector(qt$mu)
  betweenness <- betweenness(graph, directed=TRUE)
  df <- data.frame(serviceRate, betweenness);

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


des.queueing.delay.betweenness.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-delay-plot.eps", onefile=FALSE)
  }

  delay <- as.vector(qt$r)
  betweenness <- betweenness(graph, directed=TRUE)
  df <- data.frame(delay, betweenness);

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


des.queueing.events.betweenness.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-betweenness-vs-events.eps", onefile=FALSE)
  }

  events <- as.vector(qt$upsilon)
  betweenness <- betweenness(graph, directed=TRUE)
  df <- data.frame(events, betweenness);

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


des.queueing.numEvents.degree.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-vs-numEvents-plot.eps", onefile=FALSE)
  }

  expectedAverageNumEvents <- as.vector(qt$L)
  degree <- degree(graph, mode="in")
  df <- data.frame(expectedAverageNumEvents, degree);

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


des.queueing.utilisation.degree.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-vs-util-plot.eps", onefile=FALSE)
  }

  utilisation <- as.vector(qt$rho)
  degree <- degree(graph, mode="in")
  df <- data.frame(utilisation, degree);

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


des.queueing.delay.degree.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-vs-delay-plot.eps", onefile=FALSE)
  }

  delay <- as.vector(qt$r)
  degree <- degree(graph, mode="in")
  df <- data.frame(delay, degree);

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


des.queueing.delay.utilisation.plot <- function(qt, graph, ps=TRUE) {
  if (ps) {
    postscript("queueing-delay-vs-customers-evo-plot.eps", onefile=FALSE)
  }

  delay <- as.vector(qt$r)
  utilisation <- as.vector(qt$rho)
  df <- data.frame(delay, utilisation);

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


library(igraph)
library(ggplot2)

graphs <- dir()

des.queueing.main(qt, graph)
