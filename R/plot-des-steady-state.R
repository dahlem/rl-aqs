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
## Created: 08.02.2009

## This script generates graphs for the steady-state. Those graphs include
## - power-law for service, aggregated arrival, expected num events in queue
## - distance from randomness (with and without nodes with less than 2 out-degree)
## - arrival/service vs betweenness
## - average number of events vs betweenness
## - percent stable
## - Q-value distribution for all nodes where the out-degree > 1
## - distance from randomness distribution for all nodes where the out-degree > 1

## Trace a single node with max out-degree
## - distance from randomness evolution
## - table with shortest paths for each of its neighbours

library(desGraph)

des.steady.power.law.degree <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-degree-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(degree=c(), cumFreq=c(), size=c())

  degree <- degree(graph, mode="in")
  x <- sort(degree)
  n <- length(x)
  vals <- unique(x)
  y <- tabulate(match(x, vals))/n
  df <- rbind(df,
              data.frame(degree=vals,
                         cumFreq=y,
                         size=rep(vcount(graph), length(y))))

  df <- df[df$degree > 0,]

  p <- ggplot(df, aes(x=degree, y=cumFreq))
  p <- p + geom_point()
  if (fit) {
    pl <- plfit(df$degree)
    dfPL <- data.frame(x=pl$xmin:max(df$degree),y=10*(pl$xmin:max(df$degree))^(-pl$alpha+1))
    dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequencies")
  p <- p + scale_x_continuous("Vertex In-Degree")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.steady.power.law.arrival <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-total-arrival-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(lambda=c(), counts=c(), size=c())

  lambda <- as.vector(des.queueing.lambda.vec(graph))
  n <- length(lambda)
  vals <- seq(0,ceiling(max(lambda)), ceiling(max(lambda)) / 10000)
  y <- hist(lambda, breaks = vals, plot = FALSE)$counts
  df <- rbind(df,
              data.frame(lambda=vals[-1],
                         counts=y,
                         size=rep(vcount(graph), length(y))))

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=lambda, y=cumFreq))
  if (fit) {
    pl <- plfit(df$lambda)
    dfPL <- data.frame(x=pl$xmin:max(df$lambda),y=10*(pl$xmin:max(df$lambda))^(-pl$alpha))
    dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Arrival Rates")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.power.law.service <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-service-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(mu=c(), counts=c(), size=c())

  mu <- V(graph)$service_rate
  n <- length(mu)
  vals <- seq(0,ceiling(max(mu)), ceiling(max(mu)) / 10000)
  y <- hist(mu, breaks = vals, plot = FALSE)$counts
  df <- rbind(df,
              data.frame(mu=vals[-1],
                         counts=y,
                         size=rep(vcount(graph), length(y))))

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=mu, y=cumFreq))
  if (fit) {
    pl <- plfit(df$mu)
    dfPL <- data.frame(x=pl$xmin:max(df$mu),y=10*(pl$xmin:max(df$mu))^(-pl$alpha))
    dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Service Rates")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.power.law.delay <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-avg-delay-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(r=c(), counts=c(), size=c())

  r <- V(graph)$average_delay_in_queue
  n <- length(r)
  vals <- seq(0,ceiling(max(r)), ceiling(max(r)) / 10000)
  y <- hist(r, breaks = vals, plot = FALSE)$counts
  df <- rbind(df,
              data.frame(r=vals[-1],
                         counts=y,
                         size=rep(vcount(graph), length(y))))

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=r, y=cumFreq))
  if (fit) {
    pl <- plfit(df$r)
    dfPL <- data.frame(x=pl$xmin:max(df$r),y=10*(pl$xmin:max(df$r))^(-pl$alpha))
    dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Average Delay")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.power.law.utilisation <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-utilisation-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(rho=c(), counts=c(), size=c())

  rho <- V(graph)$utilisation
  n <- length(rho)
  vals <- seq(0,ceiling(max(rho)), ceiling(max(rho)) / 10000)
  y <- hist(rho, breaks = vals, plot = FALSE)$counts
  df <- rbind(df,
              data.frame(rho=vals[-1],
                         counts=y,
                         size=rep(vcount(graph), length(y))))

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=rho, y=cumFreq))
  if (fit) {
    pl <- plfit(df$rho)
    dfPL <- data.frame(x=pl$xmin:max(df$rho),y=10*(pl$xmin:max(df$rho))^(-pl$alpha))
    dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Utilisaton")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.distance.randomness <- function(prefix, graphs, stopTime, mode, thres, se=FALSE, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-mode", mode, "-thres_", thres, "_evo-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1)
  df <- data.frame(time=rep(0, graphs), distance=rep(0, graphs))

  if (se) {
    df <- cbind(df, ucl=rep(0, graphs), lcl=rep(0, graphs))
  }
  
  df$time = seq(0, (graphs - 1)) * interval

  for (i in 0:(graphs - 1)) {
    file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
    graph = read.graph(file, format="graphml")

    if (se) {
      graphInfo <- des.graph.node.jackknife.sigma(graph, mode, thres)
      df$distance[i+1] <- graphInfo$meanDistance
      df$ucl[i+1] <- df$distance[i+1] + 1.96 * graphInfo$sigmaDistance/sqrt(graphInfo$n)
      df$lcl[i+1] <- df$distance[i+1] - 1.96 * graphInfo$sigmaDistance/sqrt(graphInfo$n)
    } else {
      graphInfo <- des.graph.info(graph, mode, thres)
      df$distance[i+1] <- mean(graphInfo$distance)
    }
  }

  p <- ggplot(df, aes(x=time, y=distance))
  p <- p + geom_line()
  if (se) {
    p <- p + geom_line(aes(x=time, y=ucl), linetype=2, color="lightgrey")
    p <- p + geom_line(aes(x=time, y=lcl), linetype=2, color="lightgrey")
  }
  p <- p + scale_y_continuous("Distance from Random Behaviour")
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw()
  p <- p + stat_smooth(se=FALSE)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.steady.indegree.arrival <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-total-arrival-indegree-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(lambda=c(), indegree=c())

  lambda <- as.vector(des.queueing.lambda.vec(graph))
  indegree <- degree(graph, mode="in")
  df <- rbind(df,
              data.frame(lambda=lambda,
                         indegree=indegree))

  p <- ggplot(df, aes(y=lambda, x=indegree))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("In-Degree")
  p <- p + scale_y_continuous("Arrival Rate")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.indegree.service <- function(prefix, graph, ps=TRUE, fit=FALSE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-total-service-indegree-plot.eps", sep=""), onefile=FALSE)
  }

  df <- data.frame(mu=c(), indegree=c())

  mu <- V(graph)$service_rate
  indegree <- degree(graph, mode="in")
  df <- rbind(df,
              data.frame(mu=mu,
                         indegree=indegree))

  p <- ggplot(df, aes(y=mu, x=indegree))
  p <- p + layer(geom = "point")
  p <- p + scale_x_continuous("In-Degree")
  p <- p + scale_y_continuous("Service Rate")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.stable <- function(prefix, graphs, stopTime, epsilon, se=FALSE, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-epsilon_", epsilon, "-evo-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1)
  df <- data.frame(time=rep(0, graphs), rhoMean=rep(0, graphs), stable=rep(0, graphs))
  df$time = seq(0, (graphs - 1)) * interval

  for (i in 0:(graphs - 1)) {
    file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
    print(paste("Read graph", file))
    graph = read.graph(file, format="graphml")

    Q <- des.rl.epsilon.queueing.Q.matrix(graph, epsilon)
    rho <- des.queueing.rho.vec(graph, Q)
    df$rhoMean[i+1] <- mean(rho)
    stab <- rho[rho >= 1]
    if (length(stab) > 0) {
      df$stable[i+1] <- 0
    } else {
      df$stable[i+1] <- 1
    }
  }

  p <- ggplot(df, aes(x=time, y=rhoMean))
  p <- p + geom_point(aes(shape = factor(stable)), size = 4)
  p <- p + scale_y_continuous("Network Stability in %")
  p <- p + scale_x_continuous("Time")
  p <- p + scale_shape("Stable")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.qval.power.law <- function(prefix, graph, fit=FALSE, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-qval-power-law-plot.eps", sep=""), onefile=FALSE)
  }

  dfFiltered <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  dfFiltered <- dfFiltered[dfFiltered$degree > 1,]
  
  df <- data.frame(qval=c(), counts=c(), size=c())

  qval <- c()
  for (i in dfFiltered$vertices) {
    qval <- c(qval, -E(graph)[from(i)]$q_value)
  }
  
  n <- length(qval)
  vals <- seq(0,ceiling(max(qval)), ceiling(max(qval)) / 10000)
  y <- hist(qval, breaks = vals, plot = FALSE)$counts
  df <- rbind(df,
              data.frame(qval=vals[-1],
                         counts=y,
                         size=rep(vcount(graph), length(y))))

  ## only take the bins that actually have values
  df <- df[df$counts > 0,]

  ## add the cumulative frequency
  df <- cbind(df, cumFreq=(df$counts/sum(df$counts)))
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=qval, y=cumFreq))
  if (fit) {
    pl <- plfit(df$qval)
    dfPL <- data.frame(x=pl$xmin:max(df$qval),y=10*(pl$xmin:max(df$qval))^(-pl$alpha))
    dfPL <- dfPL[dfPL$y < max(df$cumFreq) & dfPL$y > min(df$cumFreq),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
  }
  p <- p + coord_trans(x = "log10", y = "log10")
  p <- p + scale_y_continuous("Cumulative Frequency")
  p <- p + scale_x_continuous("Q-Values")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.qval.dist <- function(prefix, graph, fit=FALSE, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "steady-state-qval-dist-plot.eps", sep=""), onefile=FALSE)
  }

  dfFiltered <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  dfFiltered <- dfFiltered[dfFiltered$degree > 1,]
  
  qval <- c()
  for (i in dfFiltered$vertices) {
    qval <- c(qval, -E(graph)[from(i)]$q_value)
  }

  df <- data.frame(qval=qval)
  
  p <- ggplot()
  p <- p + geom_density(data=df, aes(x=qval), adjust=5)
  p <- p + scale_x_continuous("Q-Values")
  p <- p + scale_y_continuous("Density")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.distance.randomness.hist <- function(prefix, graph, mode, thres, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-mode", mode, "-thres_", thres, "_hist-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  graphInfo <- des.graph.info(graph, mode, thres)
  df <- data.frame(distance=graphInfo$distance)

  p <- ggplot(df, aes(x=distance))
  p <- p + geom_histogram()
  p <- p + scale_x_continuous("Distance from Random Behaviour")
  p <- p + scale_y_continuous("Density")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.distance.randomness.trace <- function(prefix, graphs, mode, thres, numNodes=5, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-mode", mode, "-thres_", thres, "_trace-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  ## which node to trace
  dfFiltered <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  dfFiltered <- dfFiltered[order(dfFiltered$degree, decreasing=TRUE),] 
  nodes <- dfFiltered$vertices[1:numNodes]
  
  interval <- stopTime / (graphs - 1)
  df <- data.frame()

  for (n in nodes) {
    dist <- c()
    for (i in 0:(graphs - 1)) {
      file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
      print(paste("Read graph", file))
      graph = read.graph(file, format="graphml")

      graphInfo <- des.graph.info(graph, mode, thres)
      dist <- c(dist, graphInfo[graphInfo$vertices == n,]$distance)
    }
    df <- rbind(df, data.frame(time=seq(0, (graphs - 1)) * interval,
                               distance=dist,
                               vertex=rep(n, length(dist))))
  }

  p <- ggplot(df, aes(x=time, y=distance, linetype=factor(vertex)))
  p <- p + scale_y_continuous("Distance from Random Behaviour")
  p <- p + scale_x_continuous("Time")
  p <- p + geom_smooth(se=FALSE)
  p <- p + scale_linetype("Vertex")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.qval.randomness.trace <- function(prefix, graphs, mode, thres, numNodes=5, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-mode", mode, "-thres_", thres, "_qval-trace-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  ## which node to trace
  dfFiltered <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  dfFiltered <- dfFiltered[order(dfFiltered$degree, decreasing=TRUE),] 
  nodes <- dfFiltered$vertices[1:numNodes]
  
  interval <- stopTime / (graphs - 1)
  df <- data.frame()

  for (n in nodes) {
    qval <- c()
    for (i in 0:(graphs - 1)) {
      file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
      print(paste("Read graph", file))
      graph = read.graph(file, format="graphml")

      graphInfo <- des.graph.info(graph, mode, thres)
      qval <- c(qval, graphInfo[graphInfo$vertices == n,]$greedyQVal)
    }
    df <- rbind(df, data.frame(time=seq(0, (graphs - 1)) * interval,
                               qval=qval,
                               vertex=rep(n, length(qval))))
  }

  p <- ggplot(df, aes(x=time, y=qval, linetype=factor(vertex)))
  p <- p + scale_y_continuous("Greedy Q-Value")
  p <- p + scale_x_continuous("Time")
  p <- p + geom_smooth(se=FALSE)
  p <- p + scale_linetype("Vertex")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.distance.randomness.trace.single <- function(prefix, graphs, mode, thres, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-mode", mode, "-thres_", thres, "_trace-single-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  ## which node to trace
  dfFiltered <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  dfFiltered <- dfFiltered[order(dfFiltered$degree, decreasing=TRUE),] 
  n <- dfFiltered$vertices[1]
  
  interval <- stopTime / (graphs - 1)
  df <- data.frame()

  dist <- c()
  for (i in 0:(graphs - 1)) {
    file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
    print(paste("Read graph", file))
    graph = read.graph(file, format="graphml")

    graphInfo <- des.graph.info(graph, mode, thres)
    dist <- c(dist, graphInfo[graphInfo$vertices == n,]$distance)
  }
  df <- rbind(df, data.frame(time=seq(0, (graphs - 1)) * interval,
                             distance=dist,
                             vertex=rep(n, length(dist))))

  p <- ggplot(df, aes(x=time, y=distance, linetype=factor(vertex)))
  p <- p + geom_line()
  p <- p + scale_y_continuous("Distance from Random Behaviour")
  p <- p + scale_x_continuous("Time")
  p <- p + geom_smooth()
  p <- p + scale_linetype("Vertex")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.qval.trace.single <- function(prefix, graphs, mode, thres, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-mode", mode, "-thres_", thres, "_qval-trace-single-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  ## which node to trace
  dfFiltered <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  dfFiltered <- dfFiltered[order(dfFiltered$degree, decreasing=TRUE),] 
  n <- dfFiltered$vertices[1]
  
  interval <- stopTime / (graphs - 1)
  df <- data.frame()

  qval <- c()
  for (i in 0:(graphs - 1)) {
    file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
    print(paste("Read graph", file))
    graph = read.graph(file, format="graphml")

    graphInfo <- des.graph.info(graph, mode, thres)
    qval <- c(qval, graphInfo[graphInfo$vertices == n,]$greedyQVal)
  }
  df <- rbind(df, data.frame(time=seq(0, (graphs - 1)) * interval,
                             qval=qval,
                             vertex=rep(n, length(qval))))

  p <- ggplot(df, aes(x=time, y=qval, linetype=factor(vertex)))
  p <- p + geom_line()
  p <- p + scale_y_continuous("Greedy Q-Value")
  p <- p + scale_x_continuous("Time")
  p <- p + geom_smooth()
  p <- p + scale_linetype("Vertex")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.steady.correlation.qval.sp <- function(prefix, graphs, stopTime, se=FALSE, ps=TRUE) {
  if (ps) {
    outfile <- paste(prefix, "graph-correlation-qval-sp-evo-plot.eps", sep="")
    postscript(outfile, onefile=FALSE)
  }

  interval <- stopTime / (graphs - 1)
  df <- data.frame(time=rep(0, graphs), tau=rep(0, graphs))
  df$time = seq(0, (graphs - 1)) * interval

  for (i in 1:(graphs - 1)) {
    file <- paste(prefix, "graphs/graph", i, ".gml", sep="")
    print(paste("Read graph", file))
    graph = read.graph(file, format="graphml")
    distances <- des.graph.node.qval.shortest.path(graph)
    df$tau[i+1] <- cor.test(distances$normQ, distances$normPaths, method="kendall")$estimate
  }

  ytitle <- bquote(paste("Kendall's ", tau, " of ",  group("|", d[Q], "|")[1], " and ", group("|", d[SP], "|")[1]))
  p <- ggplot(df, aes(x=time, y=tau))
  p <- p + geom_line()
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("Time")
  p <- p + scale_shape("Stable")
  p <- p + geom_smooth(se=se)
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}




simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)

## do the graphs for each replica and all simulations
for (sim in simulations$sim_num) {
  replicas <- read.csv(paste(sim, "/replica_results.dat", sep=""), header=TRUE)
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  nodes <- simulations[simulations$sim_num == sim,]$network_size
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  
  for (i in replicas$rep_num) {
    prefix <- paste(sim, "/", i, "/", sep="")
    lastGraphFile <- paste(prefix, "graphs/graph", (graphs - 1), ".gml", sep="")
    graph <- read.graph(lastGraphFile, format="graphml")
    
    fit <- FALSE
    if (nodes > 999) {
      fit <- TRUE
    }
    des.steady.power.law.degree(prefix, graph, ps=TRUE, fit=fit)
    des.steady.power.law.arrival(prefix, graph, ps=TRUE, fit=FALSE)
    des.steady.power.law.service(prefix, graph, ps=TRUE, fit=FALSE)
    des.steady.power.law.delay(prefix, graph, ps=TRUE, fit=FALSE)
    des.steady.power.law.utilisation(prefix, graph, ps=TRUE, fit=FALSE)
    des.steady.distance.randomness(prefix, graphs, stopTime, mode=1, thres=TRUE, se=FALSE, ps=TRUE)
    des.steady.indegree.arrival(prefix, graph, ps=TRUE, fit=FALSE)
    des.steady.indegree.service(prefix, graph, ps=TRUE, fit=FALSE)
    des.steady.stable(prefix, graphs, stopTime, epsilon=0.1, se=FALSE, ps=TRUE)
    des.steady.qval.power.law(prefix, graph, fit=FALSE, ps=TRUE)
    des.steady.qval.dist(prefix, graph, fit=FALSE, ps=TRUE)
    des.steady.distance.randomness.hist(prefix, graph, mode=1, thres=TRUE, ps=TRUE)
    des.steady.distance.randomness.trace(prefix, graphs, mode=1, thres=TRUE, numNodes=5, ps=TRUE)
    des.steady.qval.randomness.trace(prefix, graphs, mode=1, thres=TRUE, numNodes=5, ps=TRUE)
    des.steady.distance.randomness.trace.single(prefix, graphs, mode=1, thres=TRUE, ps=TRUE)
    des.steady.qval.trace.single(prefix, graphs, mode=1, thres=TRUE, ps=TRUE)
    des.steady.correlation.qval.sp(prefix, graphs, stopTime, se=FALSE, ps=TRUE)
  }
}
