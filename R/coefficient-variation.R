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
## Keywords: analysis of the coefficient of variation
## Created: 21.05.2010

## 1. Read the coefficient of variation for one replication
## 2. Calculate how the CV is related to the depth of the node in the graph
## 2.1 Take the mean of the CVs of all nodes having the same depth
## 2.2 Print the mean of the CVs of all nodes
## 2.3 calculate the correlation of all mean CVs

## 1. Read vertex attribute CV from graph
cna.readProp <- function(graph, FUN, ...) {
  g <- read.graph(graph, format="graphml")

  attrVals <- FUN(g, ...)

  return(attrVals)
}

cna.readCVs <- function(graphs) {
  cvs <- sapply(graphs, cna.readProp, get.vertex.attribute, name="vertex_coeff_var")
}

create.graphs <- function(dir=".", sim, rep, graphs) {
  graphs <- paste(dir, "/", sim, "/", rep, "/graphs/graph", seq(0, graphs), ".gml", sep="")
  return(graphs)
}

cv.depth <- function(cvs, depthSpec) {
  m <- matrix(0, nrow=length(depthSpec), ncol=ncol(cvs))
  
  for (i in 1:ncol(cvs)) {
    for (j in 1:length(depthSpec)) {
      m[j,i] <- mean(cvs[depthSpec[[j]], i])
    }
  }

  return(m)
}


graphs <- create.graphs(".", 1, 1, 4000)
cvs <- cna.readCVs(graphs)

depthSpec <- list(c(4), c(6), c(7))
cvsM <- cv.depth(cvs, depthSpec)

write.table(cvsM, "coefficient_variation_mean.dat", sep=",", row.names=F, col.names=paste("Depth_", 1:length(depthSpec), sep=""))

tcvsM <- t(cvsM)

rho <- cor(tcvsM, method = "spearman")
tau <- cor(tcvsM, method = "kendall")
r <- cor(tcvsM, method = "pearson")
