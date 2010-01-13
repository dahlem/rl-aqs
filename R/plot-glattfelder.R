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




vplayout <- function(Layout) {
  grid.newpage()
  pushViewport(viewport(layout = Layout))
}

subplot <- function(x, y) viewport(layout.pos.row = x, layout.pos.col = y)


des.backbone <- function(graph, vartheta, prop) {
  dat <- des.control.ownership(graph, policy="wpl", nonTopProp=prop)
  backbone <- des.backbone.control(dat, vartheta)
  etaPrime <- des.backbone.control.eta.prime(dat, vartheta)
  localControl <- des.backbone.control.local(graph, backbone, inout=1)
  return(data.frame(etaPrime, localControl))
}


des.plot.evo.eta.prime.group <- function(prefix,
                                         group=c("evo-avg-delay-control.dat"),
                                         labels=c(expression(paste(epsilon, "-greedy"))),
                                         stopTime=200000,
                                         ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-control-evo-etaPrime.eps", sep=""), width=3.5, height=2.8)
  }

  data <- read.csv(group[1])
  df <- data.frame(time=data$time,
                   etaPrime=data$etaPrime,
                   type=rep(1, length(data$time)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      df <- rbind(df, data.frame(time=data$time,
                                 etaPrime=data$etaPrime,
                                 type=rep(i, length(data$time))))
    }

    p <- ggplot(df, aes(x=time, y=etaPrime, linetype=factor(type)))
    p <- p + scale_linetype("Scenario", labels=labels)
  } else {
    p <- ggplot(df, aes(x=time, y=etaPrime))
  }

  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(paste(eta, "'", sep="")))
  p <- p + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.plot.evo.eta.prime.groups <- function(prefix,
                                          group1=c("evo-avg-delay-control.dat"),
                                          group2=c("evo-avg-delay-control.dat"),
                                          group3=c("evo-avg-delay-control.dat"),
                                          group4=c("evo-avg-delay-control.dat"),
                                          types1=c(),
                                          types2=c(),
                                          types3=c(),
                                          types4=c(),
                                          titles=c("", "", "", ""),
                                          labels=c(expression(paste(epsilon, "-greedy"))),
                                          stopTime=200000,
                                          ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-control-evo-etaPrime.eps", sep=""), width=4.872, height=3.48)
  }

  data <- read.csv(group1[1])
  df1 <- data.frame(time=data$time,
                    etaPrime=data$etaPrime,
                    type=rep(1, length(data$time)))


  if (length(group1) > 1) {
    for (i in 2:length(group1)) {
      data <- read.csv(group1[i])
      df1 <- rbind(df1, data.frame(time=data$time,
                                   etaPrime=data$etaPrime,
                                   type=rep(i, length(data$time))))
    }

    p <- ggplot(df1, aes(x=time, y=etaPrime, linetype=factor(type)))
  } else {
    p <- ggplot(df1, aes(x=time, y=etaPrime))
  }

  p1 <- p + scale_linetype_manual("Scenario", labels=labels, values=types1)
  p1 <- p1 + layer(geom = "line")
  p1 <- p1 + scale_y_continuous(expression(paste(eta, "'", sep="")))
  p1 <- p1 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[1])


  data <- read.csv(group2[1])
  df2 <- data.frame(time=data$time,
                    etaPrime=data$etaPrime,
                    type=rep(1, length(data$time)))


  if (length(group2) > 1) {
    for (i in 2:length(group2)) {
      data <- read.csv(group2[i])
      df2 <- rbind(df2, data.frame(time=data$time,
                                   etaPrime=data$etaPrime,
                                   type=rep(i, length(data$time))))
    }

    p2 <- ggplot(df2, aes(x=time, y=etaPrime, linetype=factor(type)))
  } else {
    p2 <- ggplot(df2, aes(x=time, y=etaPrime))
  }

  p2 <- p2 + scale_linetype_manual("Scenario", labels=labels, values=types2)
  p2 <- p2 + layer(geom = "line")
  p2 <- p2 + scale_y_continuous(expression(paste(eta, "'", sep="")))
  p2 <- p2 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[2])


  data <- read.csv(group3[1])
  df3 <- data.frame(time=data$time,
                    etaPrime=data$etaPrime,
                    type=rep(1, length(data$time)))


  if (length(group3) > 1) {
    for (i in 2:length(group3)) {
      data <- read.csv(group3[i])
      df3 <- rbind(df3, data.frame(time=data$time,
                                   etaPrime=data$etaPrime,
                                   type=rep(i, length(data$time))))
    }

    p3 <- ggplot(df3, aes(x=time, y=etaPrime, linetype=factor(type)))
  } else {
    p3 <- ggplot(df3, aes(x=time, y=etaPrime))
  }

  p3 <- p3 + scale_linetype_manual("Scenario", labels=labels, values=types3)
  p3 <- p3 + layer(geom = "line")
  p3 <- p3 + scale_y_continuous(expression(paste(eta, "'", sep="")))
  p3 <- p3 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[3])


  data <- read.csv(group4[1])
  df4 <- data.frame(time=data$time,
                    etaPrime=data$etaPrime,
                    type=rep(1, length(data$time)))


  if (length(group4) > 1) {
    for (i in 2:length(group4)) {
      data <- read.csv(group4[i])
      df4 <- rbind(df4, data.frame(time=data$time,
                                   etaPrime=data$etaPrime,
                                   type=rep(i, length(data$time))))
    }

    p4 <- ggplot(df4, aes(x=time, y=etaPrime, linetype=factor(type)))
  } else {
    p4 <- ggplot(df4, aes(x=time, y=etaPrime))
  }

  p4 <- p4 + scale_linetype_manual("Scenario", labels=labels, values=types4)
  p4 <- p4 + layer(geom = "line")
  p4 <- p4 + scale_y_continuous(expression(paste(eta, "'", sep="")))
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


des.plot.evo.local.control.group <- function(prefix,
                                             group=c("evo-avg-delay-control.dat"),
                                             labels=c(expression(paste(epsilon, "-greedy"))),
                                             stopTime=200000,
                                             ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-control-evo-localControl.eps", sep=""), width=3.5, height=2.8)
  }

  data <- read.csv(group[1])
  df <- data.frame(time=data$time,
                   localControl=data$localControl,
                   type=rep(1, length(data$time)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      df <- rbind(df, data.frame(time=data$time,
                                 localControl=data$localControl,
                                 type=rep(i, length(data$time))))
    }

    p <- ggplot(df, aes(x=time, y=localControl, linetype=factor(type)))
    p <- p + scale_linetype("Scenario", labels=labels)
  } else {
    p <- ggplot(df, aes(x=time, y=localControl))
  }

  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(bar(s)))
  p <- p + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}


des.plot.evo.local.control.groups <- function(prefix,
                                              group1=c("evo-avg-delay-control.dat"),
                                              group2=c("evo-avg-delay-control.dat"),
                                              group3=c("evo-avg-delay-control.dat"),
                                              group4=c("evo-avg-delay-control.dat"),
                                              types1=c(),
                                              types2=c(),
                                              types3=c(),
                                              types4=c(),
                                              titles=c("", "", "", ""),
                                              labels=c(expression(paste(epsilon, "-greedy"))),
                                              stopTime=200000,
                                              ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-control-evo-localControl.eps", sep=""), width=4.872, height=3.48)
  }

  data <- read.csv(group1[1])
  df1 <- data.frame(time=data$time,
                    localControl=data$localControl,
                    type=rep(1, length(data$time)))


  if (length(group1) > 1) {
    for (i in 2:length(group1)) {
      data <- read.csv(group1[i])
      df1 <- rbind(df1, data.frame(time=data$time,
                                   localControl=data$localControl,
                                   type=rep(i, length(data$time))))
    }

    p <- ggplot(df1, aes(x=time, y=localControl, linetype=factor(type)))
  } else {
    p <- ggplot(df1, aes(x=time, y=localControl))
  }

  p1 <- p + scale_linetype_manual("Scenario", labels=labels, values=types1)
  p1 <- p1 + layer(geom = "line")
  p1 <- p1 + scale_y_continuous(expression(bar(s)))
  p1 <- p1 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[1])


  data <- read.csv(group2[1])
  df2 <- data.frame(time=data$time,
                    localControl=data$localControl,
                    type=rep(1, length(data$time)))


  if (length(group2) > 1) {
    for (i in 2:length(group2)) {
      data <- read.csv(group2[i])
      df2 <- rbind(df2, data.frame(time=data$time,
                                   localControl=data$localControl,
                                   type=rep(i, length(data$time))))
    }

    p2 <- ggplot(df2, aes(x=time, y=localControl, linetype=factor(type)))
  } else {
    p2 <- ggplot(df2, aes(x=time, y=localControl))
  }

  p2 <- p2 + scale_linetype_manual("Scenario", labels=labels, values=types2)
  p2 <- p2 + layer(geom = "line")
  p2 <- p2 + scale_y_continuous(expression(bar(s)))
  p2 <- p2 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[2])


  data <- read.csv(group3[1])
  df3 <- data.frame(time=data$time,
                    localControl=data$localControl,
                    type=rep(1, length(data$time)))


  if (length(group3) > 1) {
    for (i in 2:length(group3)) {
      data <- read.csv(group3[i])
      df3 <- rbind(df3, data.frame(time=data$time,
                                   localControl=data$localControl,
                                   type=rep(i, length(data$time))))
    }

    p3 <- ggplot(df3, aes(x=time, y=localControl, linetype=factor(type)))
  } else {
    p3 <- ggplot(df3, aes(x=time, y=localControl))
  }

  p3 <- p3 + scale_linetype_manual("Scenario", labels=labels, values=types3)
  p3 <- p3 + layer(geom = "line")
  p3 <- p3 + scale_y_continuous(expression(bar(s)))
  p3 <- p3 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[3])


  data <- read.csv(group4[1])
  df4 <- data.frame(time=data$time,
                    localControl=data$localControl,
                    type=rep(1, length(data$time)))


  if (length(group4) > 1) {
    for (i in 2:length(group4)) {
      data <- read.csv(group4[i])
      df4 <- rbind(df4, data.frame(time=data$time,
                                   localControl=data$localControl,
                                   type=rep(i, length(data$time))))
    }

    p4 <- ggplot(df4, aes(x=time, y=localControl, linetype=factor(type)))
  } else {
    p4 <- ggplot(df4, aes(x=time, y=localControl))
  }

  p4 <- p4 + scale_linetype_manual("Scenario", labels=labels, values=types4)
  p4 <- p4 + layer(geom = "line")
  p4 <- p4 + scale_y_continuous(expression(bar(s)))
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
  graph1 <- read.graph(paste(sim, "/1/graphs/graph", 1, ".gml", sep=""), format="graphml")
  prop1 <- V(graph1)$average_delay_in_queue
  prop3 <- V(graph1)$regret_absolute
  df1 <- des.control.ownership(graph=graph1, policy="wpl", nonTopProp=prop1)
  df3 <- des.control.ownership(graph=graph1, policy="wpl", nonTopProp=prop3)
  write.csv(df1, "start-avg-delay-control.dat")
  write.csv(df3, "start-regret-absolute-control.dat")
  des.control.plot("start-avg-delay-", df1)
  des.control.plot("start-regret-absolute-", df3)
  
  graph2 <- read.graph(paste(sim, "/1/graphs/graph", graphs-1, ".gml", sep=""), format="graphml")
  prop1 <- V(graph2)$average_delay_in_queue
  prop3 <- V(graph2)$regret_absolute
  df1 <- des.control.ownership(graph=graph2, policy="wpl", nonTopProp=prop1)
  df3 <- des.control.ownership(graph=graph2, policy="wpl", nonTopProp=prop3)
  write.csv(df1, "end-avg-delay-control.dat")
  write.csv(df3, "end-regret-absolute-control.dat")
  des.control.plot("end-avg-delay-", df1)
  des.control.plot("end-regret-absolute-", df3)

  ## (3) local control pattern, eta'
  df1 <- data.frame(time=c(), localControl=c(), etaPrime=c())
  df3 <- data.frame()
  for (i in 1:(graphs-1)) {
    graphFile <- paste(sim, "/1/graphs/graph", i, ".gml", sep="")
    print(paste("Read graph ", graphFile))
    graph <- read.graph(graphFile, format="graphml")

    ## 1.1 read specified property
    prop1 <- V(graph)$average_delay_in_queue
    data1 <- des.backbone(graph, vartheta, prop1)
    df1 <- rbind(df1, data.frame(time=interval * i,
                                 localControl=data1$localControl,
                                 etaPrime=data1$etaPrime))

    prop3 <- V(graph)$regret_absolute
    data3 <- des.backbone(graph, vartheta, prop3)
    df3 <- rbind(df3, data.frame(time=interval * i,
                                 localControl=data3$localControl,
                                 etaPrime=data3$etaPrime))
  }
  write.csv(df1, "evo-avg-delay-control.dat")
  write.csv(df3, "evo-regret-absolute-control.dat")
}


message("Finished.")
