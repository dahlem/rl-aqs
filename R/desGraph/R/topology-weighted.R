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
## Keywords: weighted graph analysis
## Created: 11.09.2009

des.topo.edgelist <- function(graph) {
  edgelist <- get.edgelist(graph) + 1
  edgelist <- cbind(edgelist, E(graph)$weight)
  return(edgelist)
}


des.topo.integrate.strength <- function(graph) {
  Q <- des.queueing.Q.matrix(graph)
  lambda <- des.queueing.lambda.vec(graph, Q)
  
  for (v in V(graph)) {
    E(graph)[v %->% V(graph)[nei(v,mode="out")]]$weight <- E(graph)[v %->% V(graph)[nei(v,mode="out")]]$weight * lambda[v+1]
  }

  return(graph)
}

des.topo.cluster <- function(graphFiles) {
  cluster.coeffs <- c()
  
  for (g in graphFiles) {
    graph <- read.graph(g, format="graphml")
    g <- des.topo.integrate.strength(graph)
    edgeList <- des.topo.edgelist(g)
    cluster.coeffs <- rbind(cluster.coeffs, clustering_w(edgeList))
  }

  return(cluster.coeffs)
}

des.topo.avg.path <- function(graphFiles) {
  dists <- c()
  maxdists <- c()
  
  for (g in graphFiles) {
    graph <- read.graph(g, format="graphml")
    g <- des.topo.integrate.strength(graph)
    edgeList <- des.topo.edgelist(g)
    ed <- distance_w(edgelist)
    edv <- as.vector(ed)
    edv <- edv[!is.infinite(edv)]
    edv <- edv[!is.na(edv)]
    edv <- edv[!is.nan(edv)]
    edv <- edv * mean(edgeList[,3])
    dists <- rbind(dists, mean(edv))
    maxdists <- rbind(maxdists, max(edv))
  }

  return(list(dists, maxdists))
}


## mode 1: in strength
## mode 2: out strength
des.topo.weighted.strength <- function(W, graph, mode=c(1,2), externalArr=T) {
  if (mode==1) {
    s <- colSums(W)
    if (externalArr) {
      s <- s + V(graph)$arrival_rate
    }
    return(s)
  } else {
    return(rowSums(W))
  }
}


## mode 1: use actual weights
## mode 2: use potential weights
des.topo.weighted.Weight <- function(graph, mode=c(1,2)) {
  Q <- des.queueing.Q.matrix(graph)

  if (mode == 1) {
    lambda <- des.queueing.lambda.vec(graph, Q)
    W <- matrix(rep(as.matrix(lambda), vcount(graph)), ncol=vcount(graph), byrow=F) * Q
  } else {
    mu <- V(graph)$service_rate
    W <- matrix(rep(as.matrix(mu), vcount(graph)), ncol=vcount(graph), byrow=F) * Q
  }
}


des.topo.weighted.affinity.i <- function(i, graph, W) {
  nout <- neighborhood(graph, 1, nodes=i-1, mode="out")[[1]]
  nin <- neighborhood(graph, 1, nodes=i-1, mode="in")[[1]]

  kout <- degree(graph, v=nout, mode="out")
  kin <- degree(graph, v=nin, mode="out")

  ## first out neighbours then in neighbours
  corr <- sum(W[i,nout + 1] * kout) + sum(W[nin + 1, i] * kin)
  return(corr)
}

des.topo.weighted.affinity <- function(graph, W, externalArr=T) {
  s <- des.topo.weighted.strength(W, graph, 1, externalArr)
  knni <- sapply(1:vcount(graph), des.topo.weighted.affinity.i, graph, W)
  return(knni)
}



des.topo.corr.i <- function(i, graph, A) {
  k <- sum(degree(graph, v=neighborhood(graph, 1, nodes=i-1, mode="all")[[1]], mode="out"))
  return(k/degree(graph, v=i-1, mode="in"))
}


des.topo.corr <- function(graph) {
  A <- get.adjacency(graph, type="both", attr=NULL)
  knni <- sapply(1:vcount(graph), des.topo.corr.i, graph, A)
}


des.topo.weighted.average.k <- function(k, df) {
  return(knniave=mean(df[df$k == k,2]))
}


des.topo.weighted.average.affinity <- function(knni, k) {
  df <- data.frame(k, knni)
  df <- df[!apply(df, 1, function(y) any(is.infinite(y))),]
  df <- df[!apply(df, 1, function(y) any(is.nan(y))),]

  knnAvg <- sapply(unique(df$k), des.topo.weighted.average.k, df)
  return(data.frame(k=unique(df$k), knnAvg=knnAvg))
}


des.topo.assortativity.plots <- function(prefix, graph, externalArr=T, ps=T) {
  kSoc <- degree(graph, mode="in")
  knnSoc <- des.topo.corr(graph)
  knnSocAvg <- des.topo.weighted.average.affinity(knnSoc, kSoc)

  WSoc <- des.topo.weighted.Weight(graph, 1)
  knnSocWeighted <- des.topo.weighted.affinity(graph, WSoc, externalArr)
  knnSocWeightedAvg <- des.topo.weighted.average.affinity(knnSocWeighted, kSoc)

  knnAvg <- rbind(knnSocAvg, knnSocWeightedAvg)
  knnAvg <- cbind(knnAvg, l=c(rep(1, length(knnSocAvg$k)), rep(2, length(knnSocWeightedAvg$k))))
  knnAvg <- knnAvg[knnAvg$knnAvg > 0, ]
  knnAvg <- knnAvg[knnAvg$k > 0, ]
  
  if (ps) {
    des.postscript(paste(prefix, "-topo-assortativity.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  p <- ggplot(knnAvg, aes(x=k, y=knnAvg, shape=factor(l)))
  p <- p + geom_point()
  p <- p + scale_x_log10(expression(k['in']), limits=c(min(knnAvg$k), max(knnAvg$k)))
  p <- p + scale_y_log10(expression(paste(bar(k)[nn]^w, ", ", bar(k)[nn])))
  p <- p + scale_shape("", labels=c(expression(bar(k)[nn]), expression(bar(k)[nn]^w)))
  p <- p + theme_bw(base_size=8)

  legend <- p + opts(keep = "legend_box")

  p <- p + opts(strip.text.x= theme_blank(),
                axis.title.x = theme_blank(),
                axis.text.x = theme_blank(),
                legend.position = "none")
  
  if (length(knnSocWeightedAvg$k) < length(knnSocAvg$k)) {
    left <- knnSocWeightedAvg$k
    right <- knnSocAvg$k
    idx <- match(left, right)
  } else {
    right <- knnSocWeightedAvg$k
    left <- knnSocAvg$k
    idx <- match(left, right)
  }

  knnCompare <- data.frame(k=knnSocAvg[idx,]$k,
                           knnAvg=knnSocAvg[idx,]$knnAvg,
                           knnWeightedAvg=knnSocWeightedAvg[idx,]$knnAvg)
  knnCompare <- knnCompare[knnCompare$knnAvg > 0, ]
  knnCompare$comp <- ((knnCompare$knnWeightedAvg - knnCompare$knnAvg) / knnCompare$knnAvg) + 1

  q <- ggplot(knnCompare, aes(x=k, y=comp))
  q <- q + geom_point(shape=3)
  q <- q + geom_hline(yintercept=0)
  q <- q + scale_x_log10(expression(k['in']), limits=c(min(knnAvg$k), max(knnAvg$k)))
  q <- q + scale_y_log10(expression(paste("((", bar(k)[nn]^w - bar(k)[nn], ")/", bar(k)[nn], ")", +1)))
  q <- q + theme_bw(base_size=8)
  q <- q + opts(legend.position = "none")
  
  Layout <- grid.layout(nrow = 2, ncol = 2,
                        widths = unit(c(2,0.4), c("null", "null")),
                        heights = unit(c(1,1), c("null", "null")))

  vplayout <- function (...) {
    grid.newpage()
    pushViewport(viewport(layout=Layout))
  }
  subplot <- function(x, y) viewport(layout.pos.row=x, layout.pos.col=y)

  ## Plotting
  vplayout()

  print(p, vp=subplot(1,1))
  print(q, vp=subplot(2,1))
  print(legend, vp=subplot(1:2,2))

  if (ps) {
    dev.off()
  }
  
}

des.topo.strength.plot <- function(prefix, strength, ps=T) {
  if (ps) {
    des.postscript(paste(prefix, "-topo-strength.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  pls <- des.power.law.dist(strength)
  df <- data.frame(x=pls$x, y=pls$cumFreq)
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=x, y=y))
  p <- p + scale_x_log10("Strength") + scale_y_log10("Rank")
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}

des.topo.degree.strength.plot <- function(prefix, degree, strength,
                                          xlab=expression(paste("in-degree, ", k['in'])),
                                          ylab=expression(paste("average in-strength, ", bar(s)['in'])),
                                          ps=T) {
  if (ps) {
    des.postscript(paste(prefix, "-topo-degree-strength.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  df <- data.frame(k=degree, s=strength)
  df <- df[!apply(df, 1, function(y) any(is.infinite(y))),]
  df <- df[!apply(df, 1, function(y) any(is.nan(y))),]

  sAvg <- sapply(unique(df$k), des.topo.weighted.average.k, df)
  data <- data.frame(k=unique(df$k), s=sAvg)
  data <- data[data$k > 0, ]
  
  p <- ggplot(data=data, aes(x=k, y=s))
  p <- p + geom_point()
  p <- p + scale_x_log10(xlab) + scale_y_log10(ylab)
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.topo.weight.plot <- function(prefix, weight, ps=T) {
  if (ps) {
    des.postscript(paste(prefix, "-topo-weight.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  pls <- des.power.law.dist(weight)
  df <- data.frame(x=pls$x, y=pls$cumFreq)
  
  p <- ggplot()
  p <- p + geom_point(data=df, aes(x=x, y=y))
  p <- p + scale_x_log10("Weight") + scale_y_log10("Rank")
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}


ap <- des.topo.avg.path(graphFiles)
c <- des.topo.cluster(graphFiles)
apR <- des.topo.avg.path(graphFilesR)
cR <- des.topo.cluster(graphFilesR)
