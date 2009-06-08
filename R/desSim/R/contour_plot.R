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
## Keywords: contour plots
## Created: 26.05.2009

des.contour.plot <- function(prefix="2d-shdf", xs=c("x1", "x2"), xtitle, ytitle, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-contour", "-", xs[1], "_", xs[2], ".eps", sep=""), onefile=FALSE)
  }

  surrogate <- read.csv(paste(prefix, "-surrogate.dat", sep=""))
  data <- surrogate[,c(xs, "y")]
  names(data) <- c("x", "y", "z")
  
  p <- ggplot(data, aes(x, y, z = z))
  p <- p + geom_tile(aes(fill = z)) + stat_contour(bins = 10)
  p <- p + scale_fill_gradientn(colours=c("blue", "cyan", "yellow", "red"))
  p <- p + scale_x_continuous(xtitle)
  p <- p + scale_y_continuous(ytitle)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}
