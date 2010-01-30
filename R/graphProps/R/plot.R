## Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: plotting functions
## Created: 08.01.2010

plot.var <- function(infile, outfile, xtitle="Time", ytitle="", breaks=NULL,
                     se=TRUE, ps=TRUE) {
  if (ps) {
    des.pdf(outfile)
  }

  df <- read.csv(infile)
  df$ymin <- df$mu - df$error
  df$ymax <- df$mu + df$error
  
  p <- ggplot(df, aes(x=time, y=mu))

  if (se) {
    p <- p + geom_smooth(aes(ymin=ymin, ymax=ymax), stat="identity")
  } else {
    p <- p + geom_line()
  }
  
  p <- p + scale_y_continuous(ytitle)

  if (!is.null(breaks)) {
    p <- p + scale_x_continuous(xtitle, breaks=breaks)
  } else {
    p <- p + scale_x_continuous(xtitle)
  }
  
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

plot.var.group <- function(infiles, outfile, xtitle="Time", ytitle="", labels=c(),
                           breaks=NULL, se=TRUE, ps=TRUE) {
  if (ps) {
    des.pdf(outfile, width=3.3, height=2.8)
  }

  data <- read.csv(infiles[1])
  df <- data.frame(time=data$time,
                   mu=data$mu,
                   error=data$error,
                   type=rep(1, length(data$time)))

  if (length(infiles) > 1) {
    for (i in 2:length(infiles)) {
      data <- read.csv(infiles[i])
      df <- rbind(df, data.frame(time=data$time,
                                 mu=data$mu,
                                 error=data$error,
                                 type=rep(i, length(data$time))))
    }

    df$ymin <- df$mu - df$error
    df$ymax <- df$mu + df$error

#    df$mu <- df$mu / df$time;
#    df[!sapply(df,function(z)any(is.nan(z)))]
    p <- ggplot(df, aes(x=time, y=mu, linetype=factor(type)))
    p <- p + scale_linetype("Scenario", breaks=1:length(infiles), labels=labels)
  } else {
    df$ymin <- df$mu - df$error
    df$ymax <- df$mu + df$error

    p <- ggplot(df, aes(x=time, y=mu))
  }

  if (se) {
    p <- p + geom_smooth(aes(ymin=ymin, ymax=ymax), colour="black", stat="identity")
  } else {
    p <- p + geom_line()
  }
  
  p <- p + scale_y_continuous(ytitle)

  if (!is.null(breaks)) {
    p <- p + scale_x_continuous(xtitle, breaks=breaks)
  } else {
    p <- p + scale_x_continuous(xtitle)
  }
  
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}
