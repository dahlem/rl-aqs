## Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

library(igraph)
library(ggplot2)
library(desStats)
library(graphProps)

message("Start with the analysis of regret!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)



vplayout <- function(Layout) {
  grid.newpage()
  pushViewport(viewport(layout = Layout))
}

subplot <- function(x, y) viewport(layout.pos.row = x, layout.pos.col = y)


des.read.incentive.deviate <- function(graphName) {
  graph <- read.graph(graphName, format="graphml")
  info <- V(graph)$incentive_deviate
  return(sum(info))
}


des.read.regret.absolute <- function(graphName) {
  graph <- read.graph(graphName, format="graphml")
  regretAbsolute <- V(graph)$regret_absolute
  numEvents <- V(graph)$num_events_processed
  return(sum(regretAbsolute / numEvents, na.rm=T))
}

des.plot.incentive.deviate <- function(dir, numGraphs, stopTime, ps=TRUE) {
  graphs <- paste(dir, "/graph", seq(0, numGraphs), ".gml", sep="")
  incentives <- sapply(graphs, des.read.incentive.deviate)

  interval <- stopTime / numGraphs;
  df <- data.frame(time=rep(0, numGraphs+1), incentives=rep(0, numGraphs+1))
  df$time = seq(0, numGraphs) * interval
  df$incentives = incentives

  if (ps) {
    des.postscript(paste(dir, "/../../../regret-incentive-deviate.eps", sep=""))
  }

  write.csv(df, "incentive-deviate.dat")
  p <- ggplot(df, aes(x=time, y=incentives))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Incentive to Deviate")
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.plot.regret.absolute <- function(dir, numGraphs, stopTime, ps=TRUE) {
  graphs <- paste(dir, "/graph", seq(0, numGraphs), ".gml", sep="")
  regrets <- sapply(graphs, des.read.regret.absolute)

  interval <- stopTime / numGraphs;
  df <- data.frame(time=rep(0, numGraphs+1), regrets=rep(0, numGraphs+1))
  df$time = seq(0, numGraphs) * interval
  df$regrets = regrets

  if (ps) {
    des.postscript(paste(dir, "/../../../regret-average.eps", sep=""))
  }

  write.csv(df, "regret-average.dat")
  p <- ggplot(df, aes(x=time, y=regrets))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Average Total Regret")
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

des.plot.incentive.deviate.group <- function(prefix,
                                             group=c("incentive-deviate.dat"),
                                             labels=c(expression(paste(epsilon, "-greedy"))),
                                             stopTime=200000,
                                             ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-regret-incentive-deviate.eps", sep=""), width=3.5, height=2.8)
  }

  data <- read.csv(group[1])
  df <- data.frame(time=data$time,
                   incentives=data$incentives,
                   type=rep(1, length(data$time)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      df <- rbind(df, data.frame(time=data$time,
                                 incentives=data$incentives,
                                 type=rep(i, length(data$time))))
    }

    p <- ggplot(df, aes(x=time, y=incentives, linetype=factor(type)))
    p <- p + scale_linetype("Scenario", labels=labels)
  } else {
    p <- ggplot(df, aes(x=time, y=incentives))
  }

  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Incentive to Deviate")
  p <- p + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.plot.incentive.deviate.groups <- function(prefix,
                                              group1=c("incentive-deviate.dat"),
                                              group2=c("incentive-deviate.dat"),
                                              group3=c("incentive-deviate.dat"),
                                              group4=c("incentive-deviate.dat"),
                                              types1=c(),
                                              types2=c(),
                                              types3=c(),
                                              types4=c(),
                                              labels=c(expression(paste(epsilon, "-greedy"))),
                                              titles=c("", "", "", ""),
                                              stopTime=200000,
                                              ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-regret-incentive-deviate.eps", sep=""), width=4.872, height=3.48)
  }

  data <- read.csv(group1[1])
  df1 <- data.frame(time=data$time,
                    incentives=data$incentives,
                    type=rep(types1[1], length(data$time)))


  if (length(group1) > 1) {
    for (i in 2:length(group1)) {
      data <- read.csv(group1[i])
      df1 <- rbind(df1, data.frame(time=data$time,
                                  incentives=data$incentives,
                                  type=rep(types1[i], length(data$time))))
    }

    p <- ggplot(df1, aes(x=time, y=incentives, linetype=factor(type)))
  } else {
    p <- ggplot(df1, aes(x=time, y=incentives))
  }

  p1 <- p + scale_linetype_manual("Scenario", labels=labels, values=types1)
  p1 <- p1 + geom_line(aes(linetype=factor(type)))
  p1 <- p1 + scale_y_continuous("Incentive to Deviate")
  p1 <- p1 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[1])


  data <- read.csv(group2[1])
  df2 <- data.frame(time=data$time,
                    incentives=data$incentives,
                    type=rep(types2[1], length(data$time)))


  if (length(group2) > 1) {
    for (i in 2:length(group2)) {
      data <- read.csv(group2[i])
      df2 <- rbind(df2, data.frame(time=data$time,
                                  incentives=data$incentives,
                                  type=rep(types2[i], length(data$time))))
    }

    p2 <- ggplot(df2, aes(x=time, y=incentives, linetype=factor(type)))
  } else {
    p2 <- ggplot(df2, aes(x=time, y=incentives))
  }

  p2 <- p2 + scale_linetype_manual("Scenario", labels=labels, values=types2)
  p2 <- p2 + geom_line(aes(linetype=factor(type)))
  p2 <- p2 + scale_y_continuous("Incentive to Deviate")
  p2 <- p2 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[2])


  data <- read.csv(group3[1])
  df3 <- data.frame(time=data$time,
                    incentives=data$incentives,
                    type=rep(types3[1], length(data$time)))


  if (length(group3) > 1) {
    for (i in 2:length(group3)) {
      data <- read.csv(group3[i])
      df3 <- rbind(df3, data.frame(time=data$time,
                                   incentives=data$incentives,
                                   type=rep(types3[i], length(data$time))))
    }

    p3 <- ggplot(df3, aes(x=time, y=incentives, linetype=factor(type)))
  } else {
    p3 <- ggplot(df3, aes(x=time, y=incentives))
  }

  p3 <- p3 + scale_linetype_manual("Scenario", labels=labels, values=types3)
  p3 <- p3 + geom_line()
  p3 <- p3 + scale_y_continuous("Incentive to Deviate")
  p3 <- p3 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[3])


  data <- read.csv(group4[1])
  df4 <- data.frame(time=data$time,
                    incentives=data$incentives,
                    type=rep(types4[1], length(data$time)))


  if (length(group4) > 1) {
    for (i in 2:length(group4)) {
      data <- read.csv(group4[i])
      df4 <- rbind(df4, data.frame(time=data$time,
                                   incentives=data$incentives,
                                   type=rep(types4[i], length(data$time))))
    }

    p4 <- ggplot(df4, aes(x=time, y=incentives, linetype=factor(type)))
  } else {
    p4 <- ggplot(df4, aes(x=time, y=incentives))
  }

  p4 <- p4 + scale_linetype_manual("Scenario", labels=labels, values=types4)
  p4 <- p4 + layer(geom = "line")
  p4 <- p4 + scale_y_continuous("Incentive to Deviate")
  p4 <- p4 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p4 <- p4 + theme_bw(base_size=8)
  p4 <- p4 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[4])
  

  Layout <- grid.layout(nrow = 2, ncol = 3,
                        widths = unit(c(1, 1, 0.8), c("null", "null", "null")),
                        heights = unit(c(1, 1), c("null", "null")))


  vplayout(Layout)
  print(p1, vp = subplot(1, 1))
  print(p2, vp = subplot(1, 2))
  print(p3, vp = subplot(2, 1))
  print(p4, vp = subplot(2, 2))
  print(legend, vp = subplot(1:2, 3))

  if (ps) {
    dev.off()
  }
}


des.plot.regret.group <- function(prefix,
                                  group=c("regret-average.dat"),
                                  labels=c(expression(paste(epsilon, "-greedy"))),
                                  stopTime=200000,
                                  ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-regret.eps", sep=""), width=3.5, height=2.8)
  }

  data <- read.csv(group[1])
  df <- data.frame(time=data$time,
                   regrets=data$regrets,
                   type=rep(1, length(data$time)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      df <- rbind(df, data.frame(time=data$time,
                                 regrets=data$regrets,
                                 type=rep(i, length(data$time))))
    }

    p <- ggplot(df, aes(x=time, y=regrets, linetype=factor(type)))
    p <- p + scale_linetype("Scenario", labels=labels)
  } else {
    p <- ggplot(df, aes(x=time, y=regrets))
  }

  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Average Total Regret")
  p <- p + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}


des.plot.regret.groups <- function(prefix,
                                   group1=c("regret-average.dat"),
                                   group2=c("regret-average.dat"),
                                   group3=c("regret-average.dat"),
                                   group4=c("regret-average.dat"),
                                   types1=c(),
                                   types2=c(),
                                   types3=c(),
                                   types4=c(),
                                   titles=c("", "", "", ""),
                                   labels=c(expression(paste(epsilon, "-greedy"))),
                                   stopTime=200000,
                                   ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-regret.eps", sep=""), width=4.872, height=3.48)
  }

  data <- read.csv(group1[1])
  df1 <- data.frame(time=data$time,
                    regrets=data$regrets,
                    type=rep(types1[1], length(data$time)))


  if (length(group1) > 1) {
    for (i in 2:length(group1)) {
      data <- read.csv(group1[i])
      df1 <- rbind(df1, data.frame(time=data$time,
                                   regrets=data$regrets,
                                   type=rep(types1[i], length(data$time))))
    }

    p <- ggplot(df1, aes(x=time, y=regrets, linetype=factor(type)))
  } else {
    p <- ggplot(df1, aes(x=time, y=regrets))
  }

  p1 <- p + scale_linetype_manual("Scenario", labels=labels, values=types1)
  p1 <- p1 + layer(geom = "line")
  p1 <- p1 + scale_y_continuous("Average Total Regret")
  p1 <- p1 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[1])

  
  data <- read.csv(group2[1])
  df2 <- data.frame(time=data$time,
                    regrets=data$regrets,
                    type=rep(types2[1], length(data$time)))


  if (length(group2) > 1) {
    for (i in 2:length(group2)) {
      data <- read.csv(group2[i])
      df2 <- rbind(df2, data.frame(time=data$time,
                                   regrets=data$regrets,
                                   type=rep(types2[i], length(data$time))))
    }

    p2 <- ggplot(df2, aes(x=time, y=regrets, linetype=factor(type)))
  } else {
    p2 <- ggplot(df2, aes(x=time, y=regrets))
  }

  p2 <- p2 + scale_linetype_manual("Scenario", labels=labels, values=types2)
  p2 <- p2 + layer(geom = "line")
  p2 <- p2 + scale_y_continuous("Average Total Regret")
  p2 <- p2 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[2])

  
  data <- read.csv(group3[1])
  df3 <- data.frame(time=data$time,
                    regrets=data$regrets,
                    type=rep(types3[1], length(data$time)))


  if (length(group3) > 1) {
    for (i in 2:length(group3)) {
      data <- read.csv(group3[i])
      df3 <- rbind(df3, data.frame(time=data$time,
                                   regrets=data$regrets,
                                   type=rep(types3[i], length(data$time))))
    }

    p3 <- ggplot(df3, aes(x=time, y=regrets, linetype=factor(type)))
  } else {
    p3 <- ggplot(df3, aes(x=time, y=regrets))
  }

  p3 <- p3 + scale_linetype_manual("Scenario", labels=labels, values=types3)
  p3 <- p3 + layer(geom = "line")
  p3 <- p3 + scale_y_continuous("Average Total Regret")
  p3 <- p3 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[3])

  
  data <- read.csv(group4[1])
  df4 <- data.frame(time=data$time,
                    regrets=data$regrets,
                    type=rep(types4[1], length(data$time)))


  if (length(group4) > 1) {
    for (i in 2:length(group4)) {
      data <- read.csv(group4[i])
      df4 <- rbind(df4, data.frame(time=data$time,
                                   regrets=data$regrets,
                                   type=rep(types4[i], length(data$time))))
    }

    p4 <- ggplot(df4, aes(x=time, y=regrets, linetype=factor(type)))
  } else {
    p4 <- ggplot(df4, aes(x=time, y=regrets))
  }

  p4 <- p4 + scale_linetype_manual("Scenario", labels=labels, values=types4)
  p4 <- p4 + layer(geom = "line")
  p4 <- p4 + scale_y_continuous("Average Total Regret")
  p4 <- p4 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p4 <- p4 + theme_bw(base_size=8)
  p4 <- p4 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[4])
  

  Layout <- grid.layout(nrow = 2, ncol = 3,
                        widths = unit(c(1, 1, 0.8), c("null", "null", "null")),
                        heights = unit(c(1, 1), c("null", "null")))


  vplayout(Layout)
  print(p1, vp = subplot(1, 1))
  print(p2, vp = subplot(1, 2))
  print(p3, vp = subplot(2, 1))
  print(p4, vp = subplot(2, 2))
  print(legend, vp = subplot(1:2, 3))


  if (ps) {
    dev.off()
  }
}


## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  numReps <- simulations[simulations$sim_num == sim,]$actual_reps
  numGraphs <- (graphs - 1);

  interval <- stopTime / numGraphs;
  time = seq(0, numGraphs) * interval
  dir <- sim

  df <- get.var.snapshots(dir, numGraphs, numReps, "regret_absolute", ci=0.975, mode="total")
  df$time <- time
  write.csv(df, paste(sim, "-regret.csv", sep=""), quote=F)

  df <- get.var.snapshots(dir, numGraphs, numReps, "incentive_deviate", ci=0.975, mode="total")
  df$time <- time
  write.csv(df, paste(sim, "-incentive-deviate.csv", sep=""), quote=F)
}


message("Finished.")
