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
## Keywords: system measurements
## Created: 12.01.2009

library(desGraph)

dir <- dir(recursive=TRUE)
filtered <- grep(".gml", dir)

df <- data.frame(boostVertex=rep(0, length(filtered)),
                 boostEdge=rep(0, length(filtered)),
                 W=rep(0, length(filtered)),
                 L=rep(0, length(filtered)),
                 Rho=rep(0, length(filtered)),
                 network=rep(0, length(filtered)))
counter <- 1

for (i in filtered) {
  print(paste("Read graph: ", dir[i]))
  graph <- read.graph(dir[i], format="graphml")
  qt <- des.queueing(graph)
  df$boostVertex[counter] <- as.numeric(sub(".*_bv(\\d.\\d{4}).*", "\\1", dir[i], perl=TRUE))
  df$boostEdge[counter] <- as.numeric(sub(".*_be(\\d.\\d{4}).*", "\\1", dir[i], perl=TRUE))
  df$W[counter] <- qt$W
  df$L[counter] <- sum(qt$L)
  df$Rho[counter] <- mean(qt$rho)
  file <- basename(dir[i])
  net <- sub("(\\W*)_.*", "\\1", file, perl=TRUE)

  if (net == "rand") {
    df$network[counter] <- 2
  } else if (net == "soc") {
    df$network[counter] <- 1
  }
  
  counter <- counter + 1
}

if (file.exists("../../results.dat") == TRUE) {
  write.table(df, "system-results.dat", sep=",", col.names=FALSE, row.names=FALSE, append=TRUE, quote=FALSE)
} else {
  write.table(df, "system-results.dat", sep=",", row.names=FALSE, append=TRUE, quote=FALSE)
}

validFiles <- dir[filtered]
numReplications <- max(as.numeric(sub(".*_v(\\d).gml", "\\1", validFiles, perl=TRUE)))
numGraphs <- length(filtered) / numReplications
  
dfmean <- data.frame(boostVertex=rep(0, numGraphs),
                     boostEdge=rep(0, numGraphs),
                     meanW=rep(0, numGraphs),
                     meanL=rep(0, numGraphs),
                     meanRho=rep(0, numGraphs),
                     meanRhoDev=rep(0, numGraphs),
                     network=rep(0, numGraphs))

for (i in seq(1, numGraphs)) {
  print(paste("from", ((i - 1) * numReplications + 1), "to", (i * numReplications)))
  meanW <- mean(df$W[((i - 1) * numReplications + 1):(i * numReplications)])
  meanL <- mean(df$L[((i - 1) * numReplications + 1):(i * numReplications)])
  meanRho <- mean(df$Rho[((i - 1) * numReplications + 1):(i * numReplications)])
  RhoDev <- sd(df$Rho[((i - 1) * numReplications + 1):(i * numReplications)])
  dfmean$meanW[i] <- meanW
  dfmean$meanL[i] <- meanL
  dfmean$meanRho[i] <- meanRho
  dfmean$meanRhoDev[i] <- RhoDev
  dfmean$boostVertex[i] <- df$boostVertex[((i - 1) * numReplications + 1)]
  dfmean$boostEdge[i] <- df$boostEdge[((i - 1) * numReplications + 1)]
  dfmean$network[i] <- df$network[((i - 1) * numReplications + 1)]
}

if (file.exists("../../results.dat") == TRUE) {
  write.table(dfmean, "system-mean-results.dat", sep=",", col.names=FALSE, row.names=FALSE, append=TRUE, quote=FALSE)
} else {
  write.table(dfmean, "system-mean-results.dat", sep=",", row.names=FALSE, append=TRUE, quote=FALSE)
}

