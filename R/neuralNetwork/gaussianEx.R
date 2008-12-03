## Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Created: 03.12.2008


des.nn.gaussian.error.plot <- function(filename, outfile, title, ps=TRUE) {
  if (ps) {
    postscript(outfile, onefile=FALSE)
  }

  data1 <- read.csv(filename, header=TRUE)
  data2 <- read.csv("dataPoints.dat", header=TRUE)
  
  errors <- data.frame(x=data1$x, y=data1$error)
  points <- data.frame(x=data2$x, y=rep(0, 11))

  p <- ggplot()
  p <- p + geom_line(data=errors, aes(x=x, y=y))
  p <- p + geom_point(data=points, aes(x=x, y=y))
  p <- p + scale_y_continuous("Mean Squared Error")
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}
