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

des.queueing.main <- function(qts, graphs, factor=NULL) {
  des.queueing.arrival.hist.plot(qts)
  des.queueing.arrival.power.law.plot(qts, graphs, factor)
  des.queueing.average.response.hist.plot(qts)
  des.queueing.average.response.power.law.plot(qts, graphs, factor)
  des.queueing.degree.hist.plot(qts, graphs)
  des.queueing.degree.power.law.plot(qts, graphs, factor)
  des.queueing.service.power.law.plot(qts, graphs, factor)
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

des.queueing.arrival.power.law.plot <- function(qts, graphs, factor, ps=TRUE) {
  if (ps) {
    postscript("queueing-total-arrival-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- rep(0, length(graphs) + 1)
  for (i in seq(1, length(graphs))) {
    nodes[i+1] <- nodes[i] + vcount(graphs[[i]])
  }
  
  df <- data.frame(lambda=c(), counts=c(), size=c())

  for (i in seq(1, length(qts))) {
    lambda <- qts[[i]]$lambda
    n <- length(lambda)
    vals <- seq(0,ceiling(max(lambda)), ceiling(max(lambda)) / 10000)
    y <- hist(lambda, breaks = vals, plot = FALSE)$counts
    
    df <- rbind(df,
                data.frame(lambda=vals[-1],
                           counts=y,
                           size=rep(vcount(graphs[[i]]), length(y))))
  }

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  if (!is.null(factor)) {
    if (factor == "size") {
      p <- p + geom_point(aes(shape = factor(size)))
    } else if (factor == "max_edges") {
    } else {
      p <- p + geom_point(data=df, aes(x=lambda, y=cumFreq))
    }
  } else {
    p <- p + geom_point(data=df, aes(x=lambda, y=cumFreq))
    if (fit) {
      pl <- plfit(df$lambda)
      dfPL <- data.frame(x=pl$xmin:max(df$lambda),y=10*(pl$xmin:max(df$lambda))^(-pl$alpha))
      dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
      p <- p + geom_line(data=dfPL, aes(x=x,y=y))
    }
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Arrival Rates")
  p <- p + theme_bw()
  p <- p + scale_shape("Network Size")
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

des.queueing.degree.power.law.plot <- function(qts, graphs, factor, ps=TRUE) {
  if (ps) {
    postscript("queueing-degree-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- rep(0, length(graphs) + 1)
  for (i in seq(1, length(graphs))) {
    nodes[i+1] <- nodes[i] + vcount(graphs[[i]])
  }
  
  df <- data.frame(degree=c(), cumFreq=c(), size=c())

  for (i in seq(1, length(qts))) {
    degree <- degree(graphs[[i]], mode="in")
    x <- sort(degree)
    n <- length(x)
    vals <- unique(x)
    y <- tabulate(match(x, vals))/n
    df <- rbind(df,
                data.frame(degree=vals,
                           cumFreq=y,
                           size=rep(vcount(graphs[[i]]), length(y))))
  }

  df <- df[df$degree > 0,]

  p <- ggplot(df, aes(x=degree, y=cumFreq))
  if (!is.null(factor)) {
    if (factor == "size") {
      p <- p + geom_point(aes(shape = factor(size)))
    } else if (factor == "max_edges") {
    } else {
      p <- p + layer(geom = "point")
    }
  } else {
    p <- p + layer(geom = "point")
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Vertex In-Degree")
  p <- p + theme_bw()
  p <- p + scale_shape("Network Size")
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

des.queueing.average.response.power.law.plot <- function(qts, graphs, factor, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript("queueing-average-response-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- rep(0, length(graphs) + 1)
  for (i in seq(1, length(graphs))) {
    nodes[i+1] <- nodes[i] + vcount(graphs[[i]])
  }
  
  df <- data.frame(response=c(), cumFreq=c(), size=c())

  for (i in seq(1, length(qts))) {
    response <- qts[[i]]$w
    n <- length(response)
    vals <- seq(0,ceiling(max(response)), ceiling(max(response)) / 10000)
    y <- hist(response, breaks = vals, plot = FALSE)$counts
    
    df <- rbind(df,
                data.frame(response=vals[-1],
                           counts=y,
                           size=rep(vcount(graphs[[i]]), length(y))))
  }

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  if (!is.null(factor)) {
    if (factor == "size") {
      p <- p + geom_point(aes(shape = factor(size)))
    } else if (factor == "max_edges") {
    } else {
      p <- p + geom_point(data=df, aes(x=response, y=cumFreq))
    }
  } else {
    p <- p + geom_point(data=df, aes(x=response, y=cumFreq))
    if (fit) {
      pl <- plfit(df$response)
      dfPL <- data.frame(x=pl$xmin:max(df$response),y=10*(pl$xmin:max(df$response))^(-pl$alpha+1))
      dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
      p <- p + geom_line(data=dfPL, aes(x=x,y=y))
    }
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Average Response Time")
  p <- p + theme_bw()
  p <- p + scale_shape("Network Size")
  print(p)

  if (ps) {
    dev.off()
  }
}


des.queueing.service.power.law.plot <- function(qts, graphs, factor, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript("queueing-total-service-power-law-plot.eps", onefile=FALSE)
  }

  nodes <- rep(0, length(graphs) + 1)
  for (i in seq(1, length(graphs))) {
    nodes[i+1] <- nodes[i] + vcount(graphs[[i]])
  }
  
  df <- data.frame(mu=c(), counts=c(), size=c())

  for (i in seq(1, length(qts))) {
    mu <- qts[[i]]$mu
    n <- length(mu)
    vals <- seq(0,ceiling(max(mu)), ceiling(max(mu)) / 10000)
    y <- hist(mu, breaks = vals, plot = FALSE)$counts
    
    df <- rbind(df,
                data.frame(mu=vals[-1],
                           counts=y,
                           size=rep(vcount(graphs[[i]]), length(y))))
  }

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  if (!is.null(factor)) {
    if (factor == "size") {
      p <- p + geom_point(aes(shape = factor(size)))
    } else if (factor == "max_edges") {
    } else {
      p <- p + geom_point(data=df, aes(x=mu, y=cumFreq))
    }
  } else {
    p <- p + geom_point(data=df, aes(x=mu, y=cumFreq))
    if (fit) {
      pl <- plfit(df$mu)
      dfPL <- data.frame(x=pl$xmin:max(df$mu),y=10*(pl$xmin:max(df$mu))^(-pl$alpha+1))
      dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
      p <- p + geom_line(data=dfPL, aes(x=x,y=y))
    }
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Service Rate")
  p <- p + theme_bw()
  p <- p + scale_shape("Network Size")
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
factored <- FALSE

if (length(filtered) == 0) {
  dir <- dir(recursive=TRUE)
  filtered <- grep(".gml", dir)
  factored <- TRUE
}

qts <- list()
graphs <- list()

for (i in filtered) {
  print(paste("Read graph: ", dir[i]))
  graph <- read.graph(dir[i], format="graphml")
  qt <- des.queueing(graph)
  qts[[length(qts)+1]] = qt
  graphs[[length(graphs)+1]] = graph
}

if (factored == TRUE) {
  des.queueing.main(qts, graphs, "size")
} else {
  des.queueing.main(qts, graphs)
}



degree <- degree(graph, mode="in")
x <- sort(degree)
n <- length(x)
vals <- unique(x)
y <- tabulate(match(x, vals))/n
df <- data.frame(vals, y)
p <- ggplot(df[df$vals>0,], aes(x=vals, y=y))
p <- p + layer(geom = "point")
p <- p + coord_trans(x = "log10", y = "log10")
