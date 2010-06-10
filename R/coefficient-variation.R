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

library(igraph)

message("Start with the analysis of regret!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)


## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  reps <- simulations[simulations$sim_num == sim,]$actual_reps

  numGraphs <- (graphs - 1);
  interval <- stopTime / numGraphs;
  time = seq(0, numGraphs) * interval

  depthSpec <- list(c(4), c(6), c(7))

  graphs <- create.graphs(".", sim, 1, numGraphs)
  cvsM <- cna.readCVs(graphs)
  cvs <- cv.depth(cvsM, depthSpec)

  df1 <- data.frame(time=rep(0, numGraphs+1), cvs=rep(0, numGraphs+1))
  df1$cvs <- cvs[1,]
  df2 <- data.frame(time=rep(0, numGraphs+1), cvs=rep(0, numGraphs+1))
  df2$cvs <- cvs[2,]
  df3 <- data.frame(time=rep(0, numGraphs+1), cvs=rep(0, numGraphs+1))
  df3$cvs <- cvs[3,]
  df4 <- data.frame(time=rep(0, numGraphs+1), cvs=rep(0, numGraphs+1))
  df4$cvs <- mean(c(cvs[2,], cvs[3,]))

  for (rep in 2:reps) {
    graphs <- create.graphs(".", sim, rep, numGraphs)
    cvsM <- cna.readCVs(graphs)
    cvs <- cv.depth(cvsM, depthSpec)

    df1 <- cbind(df1, cvs[1,])
    df2 <- cbind(df2, cvs[2,])
    df3 <- cbind(df3, cvs[3,])
    df4 <- cbind(df4, mean(c(cvs[2,], cvs[3,])))
    print(paste("Finished ", rep, "replication"))
  }
  
  muCvs1 <- rowMeans(df1)
  seCvs1 <- qt(0.975, df=reps-1) * apply(df1, 1, sd) / sqrt(reps)
  muCvs2 <- rowMeans(df2)
  seCvs2 <- qt(0.975, df=reps-1) * apply(df2, 1, sd) / sqrt(reps)
  muCvs3 <- rowMeans(df3)
  seCvs3 <- qt(0.975, df=reps-1) * apply(df3, 1, sd) / sqrt(reps)
  muCvs4 <- rowMeans(df4)
  seCvs4 <- qt(0.975, df=reps-1) * apply(df4, 1, sd) / sqrt(reps)

  results1 <- data.frame(time=seq(0, numGraphs) * interval, cvs=muCvs1, seCvs1, cvs1=cvs[1,])
  results2 <- data.frame(time=seq(0, numGraphs) * interval, cvs=muCvs2, seCvs2, cvs1=cvs[2,])
  results3 <- data.frame(time=seq(0, numGraphs) * interval, cvs=muCvs3, seCvs3, cvs1=cvs[3,])
  results4 <- data.frame(time=seq(0, numGraphs) * interval, cvs=muCvs4, seCvs4, cvs1=mean(c(cvs[2,], cvs[3,])))

  results1$time <- time
  results2$time <- time
  results3$time <- time
  results4$time <- time
  write.table(results1, paste(sim, "-coefficient_variation_mean_4.dat", sep=""), sep=",", row.names=F, quote=F)
  write.table(results2, paste(sim, "-coefficient_variation_mean_6.dat", sep=""), sep=",", row.names=F, quote=F)
  write.table(results3, paste(sim, "-coefficient_variation_mean_7.dat", sep=""), sep=",", row.names=F, quote=F)
  write.table(results4, paste(sim, "-coefficient_variation_mean_6_7.dat", sep=""), sep=",", row.names=F, quote=F)

  tcvsM <- t(cvs)
  rho <- cor(tcvsM, method = "spearman")
  write.table(rho, paste(sim, "-coefficient_variation_rho.dat", sep=""), sep=",", row.names=F, quote=F)

  tau <- cor(tcvsM, method = "kendall")
  write.table(tau, paste(sim, "-coefficient_variation_tau.dat", sep=""), sep=",", row.names=F, quote=F)

  r <- cor(tcvsM, method = "pearson")
  write.table(r, paste(sim, "-coefficient_variation_r.dat", sep=""), sep=",", row.names=F, quote=F)
}


message("Finished.")

