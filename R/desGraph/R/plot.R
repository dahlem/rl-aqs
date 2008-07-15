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
## Created: 15.07.2008

des.graph.service.plot <- function(graph, filename, ps=TRUE) {
  if (ps) {
    postscript(eval.dir(filename), onefile=FALSE)
  }

  plot(graph,
       vertex.label=get.vertex.attribute(graph, "service_rate"),
       edge.label=get.edge.attribute(graph, "weight"),
       vertex.label.family="Helvetica", edge.label.family="Helvetica")

  if (ps) {
    dev.off()
  }
}

des.graph.arrival.plot <- function(graph, filename, ps=TRUE) {
  if (ps) {
    postscript(eval.dir(filename), onefile=FALSE)
  }

  plot(graph,
       vertex.label=get.vertex.attribute(graph, "arrival_rate"),
       edge.label=get.edge.attribute(graph, "weight"),
       vertex.label.family="Helvetica", edge.label.family="Helvetica")

  if (ps) {
    dev.off()
  }
}
