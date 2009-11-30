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
## Keywords: plotting
## Created: 15.07.2008



library(desGraph)
library(desInfluence)


message("Start with the analysis of dynamic behaviour!")

simulations <- read.csv("simulations.dat", header=TRUE)
print(simulations)


## (1) for all simulations
for (sim in simulations$sim_num) {
  graphs <- simulations[simulations$sim_num == sim,]$graphs
  stopTime <- simulations[simulations$sim_num == sim,]$stop_time
  vertices <- simulations[simulations$sim_num == sim,]$network_size
  interval <- stopTime / (graphs - 1);

  ## (2) plot of correlations
  ## 2.1 control index
  Cqval <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.control.prop, method="qval", policy="wpl")
  CqvalCor <- des.control.evo.corr(Cqval, method="kendall")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=CqvalCor)
  des.evo.plot("control-qval-kendall-correlation", df, k=7, ps=T, ylab=expression(tau))

  CqvalCorRho <- des.control.evo.corr(Cqval, method="pearson")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=CqvalCorRho)
  des.evo.plot("control-qval-pearson-correlation", df, k=7, ps=T, ylab=expression(rho))

  qval <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.qval.average.props)
  qvalCor <- des.control.evo.corr(qval, method="kendall")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=qvalCor)
  des.evo.plot("qval-kendall-correlation", df, k=7, ps=T, ylab=expression(tau))

  qvalCorRho <- des.control.evo.corr(qval, method="pearson")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=qvalCorRho)
  des.evo.plot("qval-pearson-correlation", df, k=7, ps=T, ylab=expression(rho))

  expertPos <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.expert.props, metric="positive")
  expertPosCor <- des.control.evo.corr(expertPos, method="kendall")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertPosCor)
  des.evo.plot("expertPos-kendall-correlation", df, k=7, ps=T, ylab=expression(tau))

  expertPosRho <- des.control.evo.corr(expertPos, method="pearson")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertPosRho)
  des.evo.plot("expertPos-pearson-correlation", df, k=7, ps=T, ylab=expression(rho))

  expertNeg <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.expert.props, metric="negative")
  expertNegCor <- des.control.evo.corr(expertNeg, method="kendall")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertNegCor)
  des.evo.plot("expertNeg-kendall-correlation", df, k=7, ps=T, ylab=expression(tau))

  expertNegRho <- des.control.evo.corr(expertNeg, method="pearson")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertNegRho)
  des.evo.plot("expertNeg-pearson-correlation", df, k=7, ps=T, ylab=expression(rho))

  expertAbs <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.expert.props, metric="absolute")
  expertAbsCor <- des.control.evo.corr(expertAbs, method="kendall")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertAbsCor)
  des.evo.plot("expertAbs-kendall-correlation", df, k=7, ps=T, ylab=expression(tau))

  expertAbsRho <- des.control.evo.corr(expertAbs, method="pearson")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertAbsRho)
  des.evo.plot("expertAbs-pearson-correlation", df, k=7, ps=T, ylab=expression(rho))

  expertNorm <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.expert.props, metric="normal")
  expertNormCor <- des.control.evo.corr(expertNorm, method="kendall")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertNormCor)
  des.evo.plot("expertNorm-kendall-correlation", df, k=7, ps=T, ylab=expression(tau))

  expertNormRho <- des.control.evo.corr(expertAbs, method="pearson")
  df <- data.frame(time=seq(1,graphs-2) * interval, y=expertNormRho)
  des.evo.plot("expertNorm-pearson-correlation", df, k=7, ps=T, ylab=expression(rho))

  ## 2.2 plot the evolution of averages
  df <- data.frame(time=seq(1,graphs-1) * interval, y=colSums(qval))
  des.evo.plot("qval-total", df, k=7, ps=T, ylab="Total Q-Value")
  df <- data.frame(time=seq(1,graphs-1) * interval, y=colMeans(qval))
  des.evo.plot("qval-mean", df, k=7, ps=T, ylab="Mean Q-Value")

  N <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, T, graphs, vertices, des.numEvents.props)
  expertPosMovingAvg <- des.control.evo.interval.averages(expertPos, N)
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colSums(expertPosMovingAvg))
  des.evo.plot("expertPosMovAvg-total", df, k=7, ps=T, ylab=expression(paste("Total"), e^P))
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colMeans(expertPosMovingAvg))
  des.evo.plot("expertPosMovAvg-mean", df, k=7, ps=T, ylab=expression(paste("Mean"), e^P))

  expertNegMovingAvg <- des.control.evo.interval.averages(expertNeg, N)
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colSums(expertNegMovingAvg))
  des.evo.plot("expertNegMovAvg-total", df, k=7, ps=T, ylab=expression(paste("Total"), e^N))
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colMeans(expertNegMovingAvg))
  des.evo.plot("expertNegMovAvg-mean", df, k=7, ps=T, ylab=expression(paste("Mean"), e^N))

  expertAbsMovingAvg <- des.control.evo.interval.averages(expertAbs, N)
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colSums(expertAbsMovingAvg))
  des.evo.plot("expertAbsMovAvg-total", df, k=7, ps=T, ylab=expression(paste("Total"), e^Abs))
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colMeans(expertAbsMovingAvg))
  des.evo.plot("expertAbsMovAvg-mean", df, k=7, ps=T, ylab=expression(paste("Mean"), e^Abs))

  expertNormMovingAvg <- des.control.evo.interval.averages(expertNorm, N)
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colSums(expertNormMovingAvg))
  des.evo.plot("expertNormMovAvg-total", df, k=7, ps=T, ylab=expression(paste("Total"), e^Nrm))
  df <- data.frame(time=seq(1,graphs-2) * interval, y=colMeans(expertNormMovingAvg))
  des.evo.plot("expertNormMovAvg-mean", df, k=7, ps=T, ylab=expression(paste("Mean"), e^Nrm))

  ## 2.3 instantaneous stability in the steady state threshold=1.0, policy=c("wpl", "epsilon", "boltzman"), epsilon, tau
  M <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, F, graphs, vertices, des.steady.state.num.stable, threshold=0.8, policy="wpl")
  df <- data.frame(time=seq(0,graphs-1) * interval, y=colSums(M))
  des.evo.plot("stability-wpl-080", df, k=7, ps=T, ylab=expression(rho))

  M <- des.control.evo.read.graphs(paste(sim, "/1/graphs", sep=""), F, F, graphs, vertices, des.steady.state.num.stable, threshold=0.8, policy="epsilon", epsilon=0.05)
  df <- data.frame(time=seq(0,graphs-1) * interval, y=colSums(M))
  des.evo.plot("stability-epsilon-080", df, k=7, ps=T, ylab=expression(rho))
}


message("Finished.")
