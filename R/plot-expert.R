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

library(ggplot2)
library(desStats)


des.plot.expert.posneg.group <- function(prefix,
                                         group=c("expertPosMovAvg-total.dat", "expertNegMovAvg-total.dat"),
                                         labels=c("+ve", "-ve"),
                                         stopTime=200000,
                                         ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-expert-mov-avg.eps", sep=""), width=2, height=1.74)
  }

  data <- read.csv(group[1])
  df <- data.frame(time=data$time,
                   y=data$y,
                   type=rep(1, length(data$time)))


  if (length(group) > 1) {
    for (i in 2:length(group)) {
      data <- read.csv(group[i])
      df <- rbind(df, data.frame(time=data$time,
                                 y=data$y,
                                 type=rep(i, length(data$time))))
    }

    p <- ggplot(df, aes(x=time, y=y, linetype=factor(type)))
    p <- p + scale_linetype("Expert", labels=labels)
  } else {
    p <- ggplot(df, aes(x=time, y=y))
  }

  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous("Total Reward")
  p <- p + scale_x_continuous("Time", breaks=c(0, stopTime/2, stopTime))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}
