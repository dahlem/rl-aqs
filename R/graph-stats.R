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
## Keywords: graph statistics
## Created: 30.01.2009


library(desGraph)

dir <- dir(recursive=TRUE)
filtered <- grep(".*bv4.9655_be1.2825.*\\.gml", dir)

df <- data.frame(graph=c(), avgPath=c(), diameter=c(), cc=c())


for (i in filtered) {
  print(paste("Read graph: ", dir[i]))
  graph <- read.graph(dir[i], format="graphml")
  result <- des.graph.sw(graph)
  df <-rbind(df,
             data.frame(graph=dir[i],
                        avgPath=result$average.path,
                        diameter=result$diameter,
                        cc=result$cluster.coefficient))
}

dfRand <- df[1:3,]
dfSoc <- df[4:6,]

meanRand <- data.frame(meanAvgPath=mean(dfRand$avgPath),
                       sdAvgPath=sd(dfRand$avgPath),
                       meanDiameter=mean(dfRand$diameter),
                       sdDiameter=sd(dfRand$diameter),
                       meanCC=mean(dfRand$cc),
                       sdCC=sd(dfRand$cc))

meanRsoc <- data.frame(meanAvgPath=mean(dfSoc$avgPath),
                       sdAvgPath=sd(dfSoc$avgPath),
                       meanDiameter=mean(dfSoc$diameter),
                       sdDiameter=sd(dfSoc$diameter),
                       meanCC=mean(dfSoc$cc),
                       sdCC=sd(dfSoc$cc))
