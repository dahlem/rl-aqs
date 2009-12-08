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
## Keywords: fix simulations
## Created: 19.11.2009


## this script needs to be executed in the simulation directory
## it gathers the q-values of all graphs at the end of each replication

## read partial simulations file and copy it
data <- read.csv("simulations.dat", header=TRUE)
write.csv(data, "simulations-partial.dat", row.names=F)

## read gathered statistics
delay <- read.csv("delay-values.dat", header=TRUE)
util <- read.csv("utilisation-values.dat", header=TRUE)
avgNumEvents <- read.csv("avgNumEvents-values.dat", header=TRUE)
avgEventInSys <- read.csv("avgEventInSystemTime-values.dat", header=TRUE)

## update the current data matrix
data$meanDelay <- delay$delayMean
data$varDelay <- delay$delayStdDev^2
data$meanAvgNumEvents <- avgNumEvents$avgNumEventsMean
data$varAvgNumEvents <- avgNumEvents$avgNumEventsStdDev^2
data$meanAvgEventInSystem <- avgEventInSys$avgEventSystemTimeMean
data$varAvgEventInSystem <- avgEventInSys$avgEventSystemTimeStdDev^2

## write data into new simulation file
write.csv(data, "simulations.dat", row.names=F)
