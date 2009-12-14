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
## Keywords: influence, degree, weighted-directed network
## Created: 22.04.2009
## Based on: The backbone of complex networks in corporations: Who is controlling
##           whom? by Glattfelder and Battiston


## "Controlling firms through the majority voting rule" by A Chapelle, A Szafarz
des.matrix.integrated <- function(A) {
  Aintegrated <- diag(diag(nrow(A)) - colSums(A)) * solve(diag(nrow(A)) - A) * A
  return(Aintegrated)
}


des.weight.matrix <- function(graph, policy=c("wpl", "epsilon", "boltzman"), epsilon, tau) {
  if (policy == "wpl") {
    Q <- des.queueing.Q.matrix(graph)
  } else if (policy == "epsilon") {
    Q <- des.rl.epsilon.queueing.Q.matrix(graph, epsilon)
  } else if (policy == "boltzman") {
    Q <- des.rl.boltzmann.queueing.Q.matrix(graph, tau)
  } else {
    error(paste("Policy", policy, "is not supported"))
  }

  lambda <- des.queueing.lambda.vec(graph, Q)

  W <- matrix(rep(as.matrix(lambda), vcount(graph)), ncol=vcount(graph), byrow=F) * Q
  WcolSums <- colSums(W)
  W <- W / matrix(rep(as.matrix(WcolSums), vcount(graph)), ncol=vcount(graph), byrow=T)
  W[is.nan(W)] <- 0
  return(W)
}


## equation (2)
des.concentration.index.j <- function(j, W) {
  sj <- sum(W[,j])^2/sum(W[,j]^2)
  return(sj)
}


## vector of equation (2)
des.concentration.index <- function(graph, W) {
  s <- sapply(V(graph)+1, des.concentration.index.j, W=W)
  s[is.nan(s)] <- 0
  return(s)
}


## calculate the control index acc. to equation (3)
## only needed, if the whole matrix is not required
des.control.index.i.j <- function(i, j, W) {
  Hij <- W[i,j]^2/sum(W[,j]^2)
  Hij[is.nan(Hij)] <- 0

  return(Hij)
}


## calculate the H matrix equation (3)
des.control.matrix <- function(W) {
  Wsquared <- W^2
  WcolsSumOfSquares <- colSums(Wsquared)

  H <- Wsquared / matrix(rep(as.matrix(WcolsSumOfSquares), ncol(W)), ncol=ncol(W), byrow=T)
  H[is.nan(H)] <- 0

  return(H)
}


des.matrix.integrated.ownership <- function(W, prop) {
  WIntegrated <- des.matrix.integrated(W)
  phi <- WIntegrated %*% as.matrix(prop)

  return(phi)
}


## equation (17)
des.matrix.integrated.control <- function(H, prop) {
  HIntegrated <- des.matrix.integrated(H)
  controlIntegrated <- HIntegrated %*% as.matrix(prop)

  return(controlIntegrated)
}


des.control.prop <- function(graph, method=c("qval", "positive", "negative", "normal", "absolute"),
                             policy=c("wpl", "epsilon", "boltzman"), epsilon, tau) {
  ## calculate the weight matrix
  W <- des.weight.matrix(graph, policy, epsilon, tau)

  ## calculate the control matrix
  H <- des.control.matrix(W)

  nonTopProp <- NULL
  if (!is.null(method)) {
    if (method == "qval") {
      nonTopProp <- des.qval.average.props(graph)
    } else if (method == "positive") {
    } else if (method == "negative") {
    } else if (method == "normal") {
    } else if (method == "absolute") {
    }
  }

  c <- des.matrix.integrated.control(H, nonTopProp)

  return(c)
}

des.control.ownership <- function(graph, policy=c("wpl", "epsilon", "boltzman"), epsilon, tau, nonTopProp=NULL) {
  ## calculate the weight matrix
  W <- des.weight.matrix(graph, policy, epsilon, tau)

  ## calculate the control matrix
  H <- des.control.matrix(W)

  HIntegrated <- des.matrix.integrated.control(H, nonTopProp)
  WIntegrated <- des.matrix.integrated.ownership(W, nonTopProp)
  vertices <- 0:(vcount(graph)-1)

  df <- data.frame(id=vertices, cint=HIntegrated, phiint=WIntegrated)
  df <- df[order(df$cint, decreasing=TRUE),]
  df$cintCumsum <- cumsum(df$cint)
  df$cintCumsumNorm <- df$cintCumsum/sum(df$cint)
  df$cintIndex <- 1:vcount(graph)/vcount(graph)

  df <- df[order(df$phiint, decreasing=TRUE),]
  df$phiintCumsum <- cumsum(df$phiint)
  df$phiintCumsumNorm <- df$phiintCumsum/sum(df$phiint)
  df$phiintIndex <- 1:vcount(graph)/vcount(graph)

  return(df)
}


des.backbone.control <- function(data, vartheta) {
  backbone <- data[data$phiintCumsumNorm < vartheta,]
  return(backbone)
}


des.backbone.control.eta.100 <- function(data) {
  backbone <- data[data$phiintCumsumNorm < 1.0,]
  etaHat <- length(backbone$phiintCumsumNorm)
  return(etaHat)
}


des.backbone.control.eta.prime <- function(data, vartheta) {
  backbone <- des.backbone.control(data, vartheta)
  etahat <- length(backbone$phiintCumsumNorm)
  eta100 <- des.backbone.control.eta.100(data)

  etaPrime <- etahat/eta100
}


des.backbone.control.local <- function(graph, backbone, inout=1) {
  W <- des.topo.weighted.Weight(graph, mode=1) ## actual weights
  strengths <- des.topo.weighted.strength(W, graph, inout, T)
  sizeBackbone <- length(backbone$id)

  localControl <- sum(strengths[backbone$id+1])/sizeBackbone
}



des.qval.average.props.i <- function(i, graph) {
  outV <- neighbors(graph, i, mode="out")
  qval <- 0

  if (length(outV) > 0) {
    probabilities <- E(graph)[from(c(i)) & to(outV)]$weight
    qvals <- E(graph)[from(c(i)) & to(outV)]$q_value

    qval <- -probabilities %*% qvals
  } else {
    expertNormal <- get.vertex.attribute(graph, "expert_normal", index=i)
    eventsProcessed <- get.vertex.attribute(graph, "num_events_processed", index=i)

    qval <- -expertNormal/eventsProcessed
  }

  return(qval)
}

des.qval.average.props <- function(graph) {
  qvals <- sapply(V(graph), des.qval.average.props.i, graph=graph)
  return(qvals)
}


des.expert.props <- function(graph, metric="positive") {
  property <- paste("expert_", metric, sep="")
  expert <- get.vertex.attribute(graph, property)
  return(expert)
}


des.numEvents.props <- function(graph, metric="positive") {
  processed <- get.vertex.attribute(graph, "num_events_processed")
  return(processed)
}


des.steady.state.num.stable <- function(graph, threshold=1.0, policy=c("wpl", "epsilon", "boltzman"), epsilon, tau) {
  if (policy == "wpl") {
    Q <- des.queueing.Q.matrix(graph)
  } else if (policy == "epsilon") {
    Q <- des.rl.epsilon.queueing.Q.matrix(graph, epsilon)
  } else if (policy == "boltzman") {
    Q <- des.rl.boltzmann.queueing.Q.matrix(graph, tau)
  } else {
    error(paste("Policy", policy, "is not supported"))
  }
  
  rho <- des.queueing.rho.vec(graph, Q)
  numUnstable <- length(which(rho > threshold))
}

des.control.plot <- function(prefix="", df, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "control-dist.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  p <- ggplot(df, aes(x=cintIndex, y=cintCumsumNorm))
  p <- p + geom_point()
  p <- p + coord_trans(x = "log")
  p <- p + scale_y_continuous(expression(vartheta))
  p <- p + scale_x_continuous(expression(eta))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.local.control.plot <- function(prefix="", df, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "-local-control.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  p <- ggplot(df, aes(x=time, y=localControl))
  p <- p + geom_line()
  p <- p + scale_y_continuous(expression(bar(s)))
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.eta.prime.plot <- function(prefix="", df, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "-eta-prime.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  p <- ggplot(df, aes(x=time, y=etaPrime))
  p <- p + geom_line()
  p <- p + scale_y_continuous(expression(paste(eta, "'", sep="")))
  p <- p + scale_x_continuous("Time")
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

## same as above
des.ownership.plot <- function(prefix="", df, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "control-dist.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  p <- ggplot(df, aes(x=phiintIndex, y=phiintCumsumNorm))
  p <- p + geom_point()
  p <- p + coord_trans(x = "log")
  p <- p + scale_y_continuous(expression(phi))
  p <- p + scale_x_continuous(expression(eta))
  p <- p + theme_bw(base_size=8)
  print(p)

  if (ps) {
    dev.off()
  }
}

des.concentration.pdf <- function(prefix="", concentration, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "concentration-power-law.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  h <- hist(concentration, breaks=length(concentration), plot=F)
  df <- data.frame(x=h$mids,y=h$density)
  filtered <- data[data$y>1,]

  p <- ggplot(filtered, aes(x=x,y=y))
  p <- p + geom_line()
  p <- p + coord_trans(x = "log", y = "log")

  p <- p + scale_y_continuous("P(s)")
  p <- p + scale_x_continuous("Concentration (s)")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.concentration.cdf <- function(prefix="", concentration, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "concentration-index-power-law.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  res <- des.power.law.dist(concentration)

  df <- data.frame(x=res$x, y=res$cumFreq)

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + geom_point()
  p <- p + coord_trans(x = "log", y = "log")
  p <- p + scale_y_continuous("P(s)")
  p <- p + scale_x_continuous("Concentration Index (s)")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}


des.control.cdf <- function(prefix="", controls, ps=TRUE, width=2.8, height=2.8, pts=8) {
  if (ps) {
    filename <- paste(prefix, "control-index-power-law.eps", sep="")
    des.postscript(filename, width, height, pointsize=pts)
  }

  res <- des.power.law.dist(controls)

  df <- data.frame(x=res$x, y=res$cumFreq)

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + geom_point()
  p <- p + coord_trans(x = "log", y = "log")
  p <- p + scale_y_continuous("P(h)")
  p <- p + scale_x_continuous("Control Index (h)")
  p <- p + theme_bw()
  print(p)

  if (ps) {
    dev.off()
  }
}

