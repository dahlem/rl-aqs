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


library(ggplot2)
library(desStats)


vplayout <- function(Layout) {
  grid.newpage()
  pushViewport(viewport(layout = Layout))
}

subplot <- function(x, y) viewport(layout.pos.row = x, layout.pos.col = y)


des.dynamics.dist.from.rand.plot.group <- function(prefix,
                                                   group=c("graph-info-distance-from-rand-evo.dat"),
                                                   labels=c(expression(paste(epsilon, "-greedy"))),
                                                   stopTime=200000, ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-dynamics-queueing-dist-from-rand-evo-plot.eps", sep=""), width=3.2, height=2.8)
  }

  data <- read.csv(group[1])
  df <- data.frame(time=data$time,
                   dist=data$dist,
                   type=rep(1, length(data$time)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      df <- rbind(df, data.frame(time=data$time,
                                 dist=data$dist,
                                 type=rep(i, length(data$time))))
    }

    p <- ggplot(df, aes(x=time, y=dist, linetype=factor(type)))
    p <- p + scale_linetype("Scenario", labels=labels)
  } else {
    p <- ggplot(df, aes(x=time, y=dist))
  }

  p <- p + geom_line()
  p <- p + scale_y_continuous("Distance")
  p <- p + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.dynamics.dist.from.rand.plot.groups <- function(prefix,
                                                    group1=c("graph-info-distance-from-rand-evo.dat"),
                                                    group2=c("graph-info-distance-from-rand-evo.dat"),
                                                    group3=c("graph-info-distance-from-rand-evo.dat"),
                                                    group4=c("graph-info-distance-from-rand-evo.dat"),
                                                    types1=c(),
                                                    types2=c(),
                                                    types3=c(),
                                                    types4=c(),
                                                    titles=c("", "", "", ""),
                                                    labels=c(expression(paste(epsilon, "-greedy"))),
                                                    stopTime=200000, ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-dynamics-queueing-dist-from-rand-evo-plots.eps", sep=""), width=4.872, height=3.48)
  }

  data <- read.csv(group1[1])
  df1 <- data.frame(time=data$time,
                    dist=data$dist,
                    type=rep(types1[1], length(data$time)))


  if (length(group1) > 1) {
    for (i in 2:length(group1)) {
      data <- read.csv(group1[i])
      df1 <- rbind(df1, data.frame(time=data$time,
                                   dist=data$dist,
                                   type=rep(types1[i], length(data$time))))
    }

    p <- ggplot(df1, aes(x=time, y=dist, linetype=factor(type)))
  } else {
    p <- ggplot(df1, aes(x=time, y=dist))
  }

  p1 <- p + scale_linetype_manual("Scenario", labels=labels, values=types1)
  p1 <- p1 + geom_line()
  p1 <- p1 + scale_y_continuous("Distance")
  p1 <- p1 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[1])


  data <- read.csv(group2[1])
  df2 <- data.frame(time=data$time,
                    dist=data$dist,
                    type=rep(types2[1], length(data$time)))


  if (length(group2) > 1) {
    for (i in 2:length(group2)) {
      data <- read.csv(group2[i])
      df2 <- rbind(df2, data.frame(time=data$time,
                                   dist=data$dist,
                                   type=rep(types2[i], length(data$time))))
    }

    p2 <- ggplot(df2, aes(x=time, y=dist, linetype=factor(type)))
  } else {
    p2 <- ggplot(df2, aes(x=time, y=dist))
  }

  p2 <- p2 + scale_linetype_manual("Scenario", labels=labels, values=types2)
  p2 <- p2 + geom_line()
  p2 <- p2 + scale_y_continuous("Distance")
  p2 <- p2 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[2])


  data <- read.csv(group3[1])
  df3 <- data.frame(time=data$time,
                    dist=data$dist,
                    type=rep(types3[1], length(data$time)))


  if (length(group3) > 1) {
    for (i in 2:length(group3)) {
      data <- read.csv(group3[i])
      df3 <- rbind(df3, data.frame(time=data$time,
                                   dist=data$dist,
                                   type=rep(types3[i], length(data$time))))
    }

    p3 <- ggplot(df3, aes(x=time, y=dist, linetype=factor(type)))
  } else {
    p3 <- ggplot(df3, aes(x=time, y=dist))
  }

  p3 <- p3 + scale_linetype_manual("Scenario", labels=labels, values=types3)
  p3 <- p3 + geom_line()
  p3 <- p3 + scale_y_continuous("Distance")
  p3 <- p3 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(plot.title = theme_text(size = 8 * 1.1),
                  legend.position = "none", title=titles[3])
  

  data <- read.csv(group4[1])
  df4 <- data.frame(time=data$time,
                    dist=data$dist,
                    type=rep(types4[1], length(data$time)))


  if (length(group4) > 1) {
    for (i in 2:length(group4)) {
      data <- read.csv(group4[i])
      df4 <- rbind(df4, data.frame(time=data$time,
                                   dist=data$dist,
                                   type=rep(types4[i], length(data$time))))
    }

    p4 <- ggplot(df4, aes(x=time, y=dist, linetype=factor(type)))
  } else {
    p4 <- ggplot(df4, aes(x=time, y=dist))
  }

  p4 <- p4 + scale_linetype_manual("Scenario", labels=labels, values=types4)
  p4 <- p4 + geom_line()
  p4 <- p4 + scale_y_continuous("Distance")
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
