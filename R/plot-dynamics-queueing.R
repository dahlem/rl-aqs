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


des.dynamics.queueing.utilisation.plot.group <- function(prefix,
                                                         group=c("soc-epsilon-dv12-de12-diagnostics.dat", "soc-epsilon-outsource-dv12-de12-diagnostics.dat"),
                                                         labels=c(expression(paste(epsilon, "-greedy")), expression(paste(epsilon, "-greedy CFA"))),
                                                         stopTime=200000, ps=TRUE) {
  if (ps) {
    des.pdf(paste(prefix, "-dynamics-queueing-diagnostic-evo-plot.pdf", sep=""), width=9.52, height=2.8)
  }

  data <- read.csv(group[1])
  sortedDat <- data[order(unlist(data[,1])), ]
  n <- length(data$g)
  interval <- stopTime / n;

  df <- data.frame(time = seq(1, n) * interval,
                   meanUtilisation = sortedDat$meanUtilisation,
                   uclUtil = sortedDat$meanUtilisation + 1.96 * sortedDat$seUtilisation,
                   lclUtil = sortedDat$meanUtilisation - 1.96 * sortedDat$seUtilisation,
                   type=rep(1, length(sortedDat$meanUtilisation)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      sortedDat <- data[order(unlist(data[,1])), ]

      df <- rbind(df, data.frame(time = seq(1, n) * interval,
                                 meanUtilisation = sortedDat$meanUtilisation,
                                 uclUtil = sortedDat$meanUtilisation + 1.96 * sortedDat$seUtilisation,
                                 lclUtil = sortedDat$meanUtilisation - 1.96 * sortedDat$seUtilisation,
                                 type=rep(i, length(sortedDat$meanUtilisation))))
    }
  } else {
  }

  p <- ggplot(df, aes(x=time, y=meanUtilisation, linetype=factor(type)))
  p1 <- p + geom_smooth(aes(min=lclUtil, max=uclUtil), colour="black", stat="identity")
  p1 <- p1 + scale_y_continuous("Mean Utilisation")
  p1 <- p1 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p1 <- p1 + scale_linetype("Scenario", labels=labels)
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(legend.position = "none")

  data <- read.csv(group[1])
  sortedDat <- data[order(unlist(data[,1])), ]
  df1 <- data.frame(time = seq(1, n) * interval,
                    meanResponse = sortedDat$meanResponse,
                    uclResponse = sortedDat$meanResponse + 1.96 * sortedDat$seResponse,
                    lclResponse = sortedDat$meanResponse - 1.96 * sortedDat$seResponse,
                    type=rep(1, length(sortedDat$meanResponse)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      sortedDat <- data[order(unlist(data[,1])), ]

      df1 <- rbind(df1, data.frame(time = seq(1, n) * interval,
                                   meanResponse = sortedDat$meanResponse,
                                   uclResponse = sortedDat$meanResponse + 1.96 * sortedDat$seResponse,
                                   lclResponse = sortedDat$meanResponse - 1.96 * sortedDat$seResponse,
                                   type=rep(i, length(sortedDat$meanResponse))))
    }
  } else {
  }

  p2 <- ggplot(df1, aes(x=time, y=meanResponse, linetype=factor(type)))
  p2 <- p2 + geom_smooth(aes(min=lclResponse, max=uclResponse), colour="black", stat="identity")
  p2 <- p2 + scale_y_continuous("Mean Response")
  p2 <- p2 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p2 <- p2 + scale_linetype("Scenario", labels=labels)
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(legend.position = "none")

  
  data <- read.csv(group[1])
  sortedDat <- data[order(unlist(data[,1])), ]
  df2 <- data.frame(time = seq(1, n) * interval,
                    meanDelay = sortedDat$meanDelay,
                    uclDelay = sortedDat$meanDelay + 1.96 * sortedDat$seDelay,
                    lclDelay = sortedDat$meanDelay - 1.96 * sortedDat$seDelay,
                    type=rep(1, length(sortedDat$meanDelay)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      sortedDat <- data[order(unlist(data[,1])), ]

      df2 <- rbind(df2, data.frame(time = seq(1, n) * interval,
                                   meanDelay = sortedDat$meanDelay,
                                   uclDelay = sortedDat$meanDelay + 1.96 * sortedDat$seDelay,
                                   lclDelay = sortedDat$meanDelay - 1.96 * sortedDat$seDelay,
                                   type=rep(i, length(sortedDat$meanDelay))))
    }
  } else {
  }

  p3 <- ggplot(df2, aes(x=time, y=meanDelay, linetype=factor(type)))
  p3 <- p3 + geom_smooth(aes(min=lclDelay, max=uclDelay), colour="black", stat="identity")
  p3 <- p3 + scale_y_continuous("Mean Delay")
  p3 <- p3 + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p3 <- p3 + scale_linetype("Scenario", labels=labels)
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(legend.position = "none")

  Layout <- grid.layout(nrow = 1, ncol = 4,
                        widths = unit(c(1, 1, 1, .4), c("null", "null", "null", "null")),
                        heights = unit(c(3.4), c("null")))
  
  vplayout(Layout)
  print(p1, vp = subplot(1, 1))
  print(p2, vp = subplot(1, 2))
  print(p3, vp = subplot(1, 3))
  print(legend, vp = subplot(1, 4))

  if (ps) {
    dev.off()
  }

}
