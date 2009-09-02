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
## Keywords: dynamic influence, degree, weighted-directed network
## Created: 01.09.2009
## Based on: The backbone of complex networks in corporations: Who is controlling
##           whom? by Glattfelder and Battiston

## 1. read graphs
des.control.evo.read.graphs <- function(dir=".", graphs, vertices, FUN) {
  m <- cbind(matrix(0, nrow=vertices, ncol=0))
  
  for (i in 0:(graphs-1)) {
    graphFile <- paste(dir, "/graph", i, ".gml", sep="")
    print(paste("Read graph ", graphFile))
    graph <- read.graph(graphFile, format="graphml")

    ## 1.1 read specified property
    attrVals <- FUN(graph)
    m <- cbind(m, matrix(log(attrVals), nrow=vertices))
  }

  m <- m[,-1]
  m[is.nan(m)] <- 0
  m[is.infinite(m)] <- 0
  
  return(m)
}


## 2. calculate the log return
des.control.evo.log.return <- function(data) {
  m <- data[,2:ncol(data)] - data[,1:(ncol(data)-1)]
  return(m)
}


## calculate the percent log return
des.control.evo.percent.log.return <- function(data, start, end=(ncol(data)-1)) {
  p <- (data[,end] - data[,start]) / data[,start]
  return(p)
}


## 3. calculate the correlation
des.control.evo.corr <- function(data) {
  R <- cor(data, method="pearson")
  R[is.na(R)] <- 0
  return(R)
}

## 4. calculate the distance matrix
des.control.evo.dist <- function(R) {
  n <- ncol(R)
  D <- matrix(rep(0, n*n), n, n)

  for (i in 1:(n-1)) {
    for (j in (i+1):n) {
      D[i, j] <- sqrt(2 * (1 - R[i, j]))
##      D[i, j] <- 1 - R[i, j]^2
      D[j, i] <- D[i, j]
    }
  }

  return(D)
}


## 5. create the MST
des.control.evo.graph <- function(D) {
  ## remove nans
  graph <- graph.adjacency(D, mode="undirected", weighted=TRUE)

  return(graph)
}


des.control.evo.mst <- function(graph) {
  mst <- minimum.spanning.tree(graph)
  return(mst)
}


## 6. give the root vertex with maximum absolute change
des.control.evo.root.vertex <- function(M, start, end=(ncol(M)-1)) {
  percentRet <- des.control.evo.percent.log.return(M, start, end)
  rootVert <- which.max(abs(percentRet))

  return(rootVert)
}


des.control.evo.layout <- function(mst) {
  ## first create a circular layout
  layout <- layout.reingold.tilford(mst, circular=T, root=which.max(degree(mst))-1)

  ## normalise
  layout1 <- layout.norm(layout, -1, 1, -1, 1)

  ## make a few optimisation steps to avoid overlapping vertices
  layout2 <- layout.graphopt(mst, niter=10, start=layout1, max.sa.movement=1/200)

  ## scale to a large area
  layout3 <- layout.norm(layout2, -1000000, 1000000, -1000000, 1000000)
  
  return(layout3)
}


## 7. plot graph
des.control.evo.plot.graph <- function(mst, layout, M,
                                       start=1,
                                       end=(ncol(M)-1),
                                       cutoff=c(-.25, -.1, 0, .1, .25),
                                       colors=c("red", "orange", "yellow", "green", "lightblue", "darkblue"),
                                       vsize=3, vcex=0.5
                                       ) {
  percentRet <- des.control.evo.percent.log.return(M, start, end)
  colorSpec <- rep("darkblue", length(percentRet))
  
  vertexLabelSpec <- seq(1, length(percentRet))

  for (i in 1:length(percentRet)) {
    for (j in 1:length(cutoff)) {
      if (is.nan(percentRet[i])) {
        colorSpec[i] = "white"
        break
      }
      if (percentRet[i] <= cutoff[j]) {
        colorSpec[i] <- colors[j]
        break
      }
    }
  }

  plot.igraph(mst, layout=layout, vertex.color=colorSpec, vertex.label=vertexLabelSpec,
              vertex.size=vsize, vertex.label.cex=vcex, main=paste("From", start, "to", end),
              rescale=F, asp=F, xlim=c(min(layout[,1]), max(layout[,1])),
              ylim=c(min(layout[,2]), max(layout[,2])))
}
