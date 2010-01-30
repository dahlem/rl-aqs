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
## Keywords: graph, analysis, retrieve properties
## Created: 08.01.2010

get.var.nodes <- function(graph, var) {
  attributes <- get.vertex.attribute(graph, var)
  return(attributes)
}

get.var.graph <- function(file, var, mode=c("total", "mean")) {
  print(paste("Read graph: ", file, sep=""))
  graph <- read.graph(file, format="graphml")
  attributes <- get.var.nodes(graph, var)
  
  if (mode == "total") {
    return(sum(attributes))
  } else if (mode == "mean") {
    return(mean(attributes))
  }
}

## files: replications
get.var.replicas <- function(files, var, ci=0.975, mode=c("total", "mean")) {
  attribute <- sapply(files, get.var.graph, var, mode)
  n <- length(attribute)
  mu <- mean(attribute)
  error <- qt(ci,df=n-1) * sd(attribute)/sqrt(n)

  return(data.frame(mu, error))
}

## dir: simulation dir
get.var.snapshots <- function(dir, numGraphs, numRep, var, ci=0.975, mode=c("total", "mean")) {
  df <- data.frame(index=c(), mu=c(), error=c())
  
  for (i in 0:numGraphs) {
    files <- paste(dir, "/", 1:numRep, "/graphs/graph", i, ".gml", sep="")
    data <- get.var.replicas(files, var, ci, mode)
    df <- rbind(df, data.frame(index=i, mu=data$mu, error=data$error))
  }

  return(df)
}
