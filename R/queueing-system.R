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
## Keywords: system measurements
## Created: 12.01.2009

library(desGraph)

dir <- dir(recursive=TRUE)
filtered <- grep(".gml", dir)
factored <- TRUE

df <- data.frame(graphs=rep("", length(filtered)),
                 W=rep(0, length(filtered)),
                 L=rep(0, length(filtered)),
                 pf=rep(0, length(filtered)))
counter <- 1

for (i in filtered) {
  print(paste("Read graph: ", dir[i]))
  graph <- read.graph(dir[i], format="graphml")
  qt <- des.queueing(graph)
  df$graphs[counter] <- dir[i]
  df$W[counter] <- qt$W
  df$L[counter] <- qt$L
  df$pf[counter] <- qt$pf
  
  counter <- counter + 1
}

