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
## Created: 12.01.2009

des.network.radial.plot <- function(data, filename, ps=TRUE) {
  if (ps) {
    postscript(filename, onefile=FALSE, paper="special", width=7, height=7)
  }

  a <- as.character(max(data$degree))
  b <- strsplit(a, "")
  digits <- length(b[[1]])
  breaks <- 10^(digits - 1) *
    seq(0,
        round(max(data$degree) / (10^(digits-1))),
        round(max(data$degree) / (10^(digits-1)) / 5))
  labels <- max(data$degree) - breaks
  
  p <- ggplot(data, aes(x=rand, y=invdegree)) + coord_polar()
  p <- p + geom_point()
  p <- p + scale_y_continuous("In-Degree", breaks=breaks, labels=labels)
  p <- p + scale_x_continuous("")
  p <- p + opts(title="Radial Plot of the Vertex In-Degree")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


library(desGraph)

dir <- dir(recursive=TRUE)
filtered <- grep(".gml", dir)

for (i in filtered) {
  print(paste("Read graph: ", dir[i]))
  graph <- read.graph(dir[i], format="graphml")
  degree <- degree(graph, mode="in")
  rand <- runif(vcount(graph))
  df <- data.frame(degree=degree, pos=rand)
  df$invdegree <- max(df$degree) - df$degree
  df$loginvdegree <- log(df$invdegree)

  filename <- paste(strsplit(dir[i], ".gml")[[1]][1], ".eps", sep="")

  des.network.radial.plot(df, filename)
}
