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
## Keywords: graph, analysis, degree, scale-free, power-law
## Created: 15.07.2008

des.graph.degree <- function(graph, filename, ps=TRUE, from=NULL,
                             xoff=50, xmin=5, legend.text="1000 Nodes") {
  if (ps) {
    postscript(eval.dir(file), onefile=FALSE)
  }

  d <- degree(graph, mode="in")
  dd <- degree.distribution(graph, mode="in", cumulative=TRUE)
  alpha <- power.law.fit(d, xmin=xmin)
  plot(dd, log="xy", xlab="Degree", ylab="Cumulative Frequency",
       main="Weighted Directed Network Evolution")

  if (!is.null(from)) {
    yoff <- 10*xoff^(-coef(alpha) + 1)
    lines(from:max(d), 10*(from:max(d))^(-coef(alpha)+1))
    text(xoff, yoff, bquote(lambda == .(base::round(coef(alpha), digits=2))),
         pos=4)
  }

  gplots::smartlegend(x="right", y="top", inset=0,
                      legend = c(legend.text),
                      fill=c("black"),
                      bg = "white")

  if (ps) {
      dev.off();
  }
}


## from "Assortative Mixing in Networks" and "Mixing patterns in networks"
## by Newman
des.graph.degree.assortivity <- function(graph) {
  M <- 1/length(E(graph))
  j <- 0
  k <- 0
  jk <- 0
  jSquared <- 0
  kSquared <- 0

  for (e in E(graph)) {
    inV <- V(graph)[to(e)]
    outV <- V(graph)[from(e)]

    ji <- (degree(graph, inV, mode="in", loops=FALSE) - 1)
    ki <- (degree(graph, outV, mode="out", loops=FALSE) - 1)

    j <- j + ji
    k <- k + ki

    jk <- jk + ji * ki

    jSquared <- jSquared + ji^2
    kSquared <- kSquared + ki^2
  }

  r <- (jk - M * j * k) / (sqrt((jSquared - M * j^2) * (kSquared - M * k^2)))

  return (r)
}


## from "Assortative Mixing in Networks" and "Mixing patterns in networks"
## by Newman
des.graph.degree.assortivity.sigma <- function(graph) {
  r <- des.graph.degree.assortivity(graph)
  sigma <- 0

  for (e in E(graph)) {
    g <- delete.edges(graph, e)
    ri <- des.graph.degree.assortivity(g)

    sigma <- sigma + (ri - r)^2
  }

  return (sigma)
}
