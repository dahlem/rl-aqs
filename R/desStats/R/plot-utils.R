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
## Keywords: plot utils
## Created: 12.02.2009

des.plot.powerlaw.fit <- function(data, fit=FALSE) {
  if (fit) {
    pl <- plfit(data$x)
    dfPL <- data.frame(x=pl$xmin:max(data$x),y=10*(pl$xmin:max(data$x))^(-pl$alpha+1))
    dfPL <- dfPL[dfPL$y < max(data$y) & dfPL$y > min(data$y),]
    p <- p + geom_line(data=dfPL, aes(x=x,y=y))
##    label <- bquote(paste(lambda, "=", .(pl$alpha)))
##    dfT <- data.frame(x=(max(data$x) - min(data$x))/2,
##                      y=10*((max(data$y) - min(data$y))/2)^(-pl$alpha+1))
##    p <- p + geom_text(data=dfT, label=paste(pl$alpha))
    return(p)
  }
}

## for one-column publications set width/height to 2.8
des.postscript <- function(filename, width=7, height=7, pointsize=12) {
  postscript(filename, onefile=FALSE, pointsize=pointsize,
             paper="special", width=width, height=height)
}
