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
    des.postscript(paste(prefix, "-contour", "-", xs[1], "_", xs[2], ".eps", sep=""))
  }

  surrogate <- read.csv(paste(prefix, "-surrogate.dat", sep=""))
  data <- surrogate[,c(xs, "y")]
  names(data) <- c("x", "y", "z")
  
  p <- ggplot(data, aes(x, y, z = z))
  p <- p + geom_tile(aes(fill = z)) + stat_contour(bins = 50)
  p <- p + scale_fill_gradientn(colours=c("blue", "cyan", "yellow", "red"))
  p <- p + scale_x_continuous(xtitle)
  p <- p + scale_y_continuous(ytitle)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}

vplayout <- function(Layout) {
  grid.newpage()
  pushViewport(viewport(layout = Layout))
}

subplot <- function(x, y) viewport(layout.pos.row = x, layout.pos.col = y)

des.contour.grid.plot <- function(prefix="2d-shdf",
                                  titles=c(expression(alpha), expression(lambda), expression(eta)),
                                  ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-contour-grid.eps", sep=""), width=6.1, height=1.74)
  }

  surrogate <- read.csv(paste(prefix, "-surrogate.dat", sep=""))

  data <- surrogate[,c("x1", "x2", "y")]
  names(data) <- c("x", "y", "z")
  
  p <- ggplot(data, aes(x, y, z = z))
  p1 <- p + geom_tile(aes(fill = z)) + stat_contour(bins = 10)
  p1 <- p1 + scale_fill_gradientn("S", colours=c("blue", "cyan", "yellow", "red"))
  p1 <- p1 + scale_x_continuous(titles[1])
  p1 <- p1 + scale_y_continuous(titles[2])
  p1 <- p1 + theme_bw(base_size=8)

  legend <- p1 + opts(keep = "legend_box")
  p1 <- p1 + opts(legend.position = "none")

  data2 <- surrogate[,c("x1", "x3", "y")]
  names(data2) <- c("x", "y", "z")
  
  p2 <- ggplot(data2, aes(x, y, z = z))
  p2 <- p2 + geom_tile(aes(fill = z)) + stat_contour(bins = 10)
  p2 <- p2 + scale_fill_gradientn("S", colours=c("blue", "cyan", "yellow", "red"))
  p2 <- p2 + scale_x_continuous(titles[1])
  p2 <- p2 + scale_y_continuous(titles[3])
  p2 <- p2 + theme_bw(base_size=8)
  p2 <- p2 + opts(legend.position = "none")
  
  data3 <- surrogate[,c("x2", "x3", "y")]
  names(data3) <- c("x", "y", "z")
  
  p3 <- ggplot(data3, aes(x, y, z = z))
  p3 <- p3 + geom_tile(aes(fill = z)) + stat_contour(bins = 10)
  p3 <- p3 + scale_fill_gradientn("S", colours=c("blue", "cyan", "yellow", "red"))
  p3 <- p3 + scale_x_continuous(titles[2])
  p3 <- p3 + scale_y_continuous(titles[3])
  p3 <- p3 + theme_bw(base_size=8)
  p3 <- p3 + opts(legend.position = "none")

  Layout <- grid.layout(nrow = 1, ncol = 4,
                        widths = unit(c(1, 1, 1, 0.5), c("null", "null", "null", "null")),
                        heights = unit(c(3.5), c("null")))


  vplayout(Layout)
  print(p1, vp = subplot(1, 1))
  print(p2, vp = subplot(1, 2))
  print(p3, vp = subplot(1, 3))
  print(legend, vp = subplot(1, 4))

  if (ps) {
    dev.off()
  }
}
