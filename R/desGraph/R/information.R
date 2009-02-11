## Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: graph, analysis, degree, information-theory
## Created: 24.07.2008

des.graph.max.info <- function(graph) {
  return(mean(des.graph.node.max.info(graph)))
}


des.graph.node.max.info <- function(graph, vertex=V(graph)) {
  outdegree <- degree(graph, v=vertex, mode="out", loops=FALSE)
  info <- des.graph.max.info.fn(outdegree)

  # replace -Inf with zeros
  info[info == -Inf] <- 0
  
  return(info)
}


des.graph.max.info.fn <- function(outdegree) {
  return(2 - 2/outdegree)
}


## mode 1: based on qval
##      2: based on weight
##      3: based on shortest paths
des.graph.info <- function(graph, mode=1, thres=FALSE, greedy=TRUE) {
  # for each vertex calculate the available information using
  # the weights of all out-edges

  df <- data.frame(vertices=V(graph)$id,
                   degree=degree(graph, mode="out"))
  
  if (mode == 1) {
    if (thres) {
      ## select only vertices with an out-degree of > 1
      df <- df[df$degree > 1,]
      dist <- sapply(df$vertices, des.graph.node.qval.info, graph=graph)
      df <- cbind(df, distance=dist)
    } else {
      ## select all vertices but the first one
      dist <- sapply(df$vertices, des.graph.node.qval.info, graph=graph)
      df <- cbind(df, distance=dist)
    }
  } else if (mode == 2) {
    ## select all vertices but the first one
    df <- df[-1,]
    dist <- sapply(df$vertices, des.graph.node.info, graph=graph)
    df <- cbind(df, distance=dist)
  } else if (mode == 3) {
    if (thres) {
      ## select only vertices with an out-degree of > 1
      df <- df[df$degree > 1,]
      dist <- sapply(df$vertices, des.graph.node.shortest.path.info, graph=graph)
      df <- cbind(df, distance=dist)
    } else {
      ## select all vertices but the first one
      df <- df[-1,]
      dist <- sapply(df$vertices, des.graph.node.shortest.path.info, graph=graph)
      df <- cbind(df, distance=dist)
    }
  }

  if (greedy) {
    greedyQVal <- sapply(df$vertices, des.graph.node.qval.greedy.info, graph=graph)
    df <- cbind(df, greedyQVal=greedyQVal)
  }
  
  return(df)
}

## mode 1: based on qval
##      2: based on weight
##      3: based on shortest paths
des.graph.node.jackknife.sigma <- function(graph, mode=1, thres=FALSE) {
  # for each vertex calculate the available information using
  # the weights of all out-edges

  df <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  result <- data.frame()
  
  if (mode == 1) {
    if (thres) {
      ## select only vertices with an out-degree of > 1
      df <- df[df$degree > 1,]
      dist <- sapply(df$vertices, des.graph.node.qval.info, graph=graph)
      resultMean <- mean(dist)
      sigma <- 0
      for (i in 1:length(df$vertices)) {
        disti <- sapply(df$vertices[-i], des.graph.node.qval.info, graph=graph)
        sigma <- sigma + (mean(disti) - resultMean)^2
      }
      result <- rbind(result, data.frame(n=length(df$vertices),
                                         meanDistance=resultMean,
                                         sigmaDistance=sqrt(sigma/(length(df$vertices) - 1))))
    } else {
      ## select all vertices but the first one
      df <- df[-1,]
      dist <- sapply(df$vertices, des.graph.node.qval.info, graph=graph)
      resultMean <- mean(dist)
      sigma <- 0
      for (i in 1:length(df$vertices)) {
        disti <- sapply(df$vertices[-i], des.graph.node.qval.info, graph=graph)
        sigma <- sigma + (mean(disti) - resultMean)^2
      }
      result <- rbind(result, data.frame(n=length(df$vertices),
                                         meanDistance=resultMean,
                                         sigmaDistance=sqrt(sigma/(length(df$vertices) - 1))))
    }
  } else if (mode == 2) {
    ## select all vertices but the first one
    df <- df[-1,]
    dist <- sapply(df$vertices, des.graph.node.info, graph=graph)
    resultMean <- mean(dist)
    sigma <- 0
    for (i in 1:length(df$vertices)) {
      disti <- sapply(df$vertices[-i], des.graph.node.info, graph=graph)
      sigma <- sigma + (mean(disti) - resultMean)^2
    }
    result <- rbind(result, data.frame(n=length(df$vertices),
                                       meanDistance=resultMean,
                                       sigmaDistance=sqrt(sigma/(length(df$vertices) - 1))))
  } else if (mode == 3) {
    if (thres) {
      ## select only vertices with an out-degree of > 1
      df <- df[df$degree > 1,]
      dist <- sapply(df$vertices, des.graph.node.shortest.path.info, graph=graph)
      resultMean <- mean(dist)
      sigma <- 0
      for (i in 1:length(df$vertices)) {
        disti <- sapply(df$vertices[-i], des.graph.node.shortest.path.info, graph=graph)
        sigma <- sigma + (mean(disti) - resultMean)^2
      }
      result <- rbind(result, data.frame(n=length(df$vertices),
                                         meanDistance=resultMean,
                                         sigmaDistance=sqrt(sigma/(length(df$vertices) - 1))))
    } else {
      ## select all vertices but the first one
      df <- df[-1,]
      dist <- sapply(df$vertices, des.graph.node.shortest.path.info, graph=graph)
      resultMean <- mean(dist)
      sigma <- 0
      for (i in 1:length(df$vertices)) {
        disti <- sapply(df$vertices[-i], des.graph.node.shortest.path.info, graph=graph)
        sigma <- sigma + (mean(disti) - resultMean)^2
      }
      result <- rbind(result, data.frame(n=length(df$vertices),
                                         meanDistance=resultMean,
                                         sigmaDistance=sqrt(sigma/(length(df$vertices) - 1))))
    }
  }

  return(result)
}


## give the qval and shortest path values for each edge
## based on those we can perform a correlation analysis to identify
## whether the learning behaviour converges to learning shortest paths
des.graph.node.qval.shortest.path <- function(graph) {
  df <- data.frame(vertices=V(graph)$id, degree=degree(graph, mode="out"))
  df <- df[df$degree > 1,]
  dfs <- lapply(df$vertices, des.graph.node.qval.sp, graph=graph)
  distances <- do.call("rbind", dfs)

  return(distances)
}

des.graph.node.info <- function(vertex, graph) {
  weights <- E(graph)[from(vertex)]$weight
  outdegree <- 1/length(weights)

  return(des.graph.info.fn(outdegree, weights))
}


des.graph.node.qval.sp <- function(vertex, graph) {
  weights <- E(graph)[from(vertex)]$q_value
  if (length(weights) == 1) {
    weights[1] = 1
  }
  sumWeights <- sum(weights)
  normWeights <- weights/sumWeights
  outdegree <- 1/length(weights)
  
  neighbours <- V(graph)[nei(vertex, mode="out")]
  sp <- shortest.paths(graph, v=neighbours, mode="out", weights=NA)
  sp[sp == Inf] = -1
  paths <- 1 + apply(sp, 1, max)

  if (length(paths) == 1) {
    paths[1] = 1
  }
  sumPaths <- sum(paths)
  normPaths <- paths/sumPaths

  return(data.frame(normQ=normWeights, normPaths))
}


des.graph.node.qval.info <- function(vertex, graph) {
  edge <- E(graph)[from(vertex)]
  if (length(edge)) {
    weights <- edge$q_value
    if (length(weights) == 1) {
      weights[1] = 1
    }
    sumWeights <- sum(weights)
    normWeights <- weights/sumWeights
    outdegree <- 1/length(weights)
  
    return(des.graph.info.fn(outdegree, normWeights))
  } else {
    return(0.0)
  }
}


des.graph.node.qval.greedy.info <- function(vertex, graph) {
  qval <- E(graph)[from(vertex)]$q_value
  if (length(qval) == 1) {
    qval[1] = 0
  }

  greedyQval <- max(qval, na.rm=TRUE)
  
  return(-greedyQval)
}


des.graph.node.shortest.path.info <- function(vertex, graph) {
  neighbours <- V(graph)[nei(vertex, mode="out")]

  sp <- shortest.paths(graph, v=neighbours, mode="out", weights=NA)
  sp[sp == Inf] = -1
  paths <- 1 + apply(sp, 1, max)

  if (length(paths) == 1) {
    paths[1] = 1
  }
  sumPaths <- sum(paths)
  normPaths <- paths/sumPaths
  outdegree <- 1/length(paths)
  
  return(des.graph.info.fn(outdegree, normPaths))
}


des.graph.info.fn <- function(outdegree, weights) {
  diff <- outdegree - weights
  return(sum(abs(diff)))
}
