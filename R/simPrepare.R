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

sims <- read.csv("./simulations.dat", header=T)

l <- length(sims$sim_num)
df <- data.frame(alpha=rep(0, l), lambda=rep(0, l), epsilon=rep(0, l),
                 delay=rep(0, l), numEvents=rep(0, l),
                 avgEventInSystemTime=rep(0, l),
                 util=rep(0, l))

for (i in sims$sim_num) {
  print(paste("Read sim: ", i))
  repFile <- paste(i, "/replica_results.dat", sep="")

  if (file.exists(repFile)) {
    reps <- read.csv(repFile, header=T, col.names=c("sim_num", "rep_num", "systemDelay", "systemAvgNumEvents", "systemTotalQ", "meanDelay", "varDelay", "meanAvgNumEvents", "varAvgNumEvents", "meanTotalQ", "varTotalQ"))
    df$alpha[i] <- sims$rlq_alpha[i]
    df$lambda[i] <- sims$rlq_lambda[i]
    df$epsilon[i] <- sims$rl_policy_epsilon[i]

    util <- read.csv("./utilisation-values.dat", header=T)
    df$util[i] <- util$utilisationMean[i]
    
    if (length(reps$systemDelay) > 0) {
      df$delay[i] <- reps$meanDelay[length(reps$meanDelay)]
      df$numEvents[i] <- reps$meanAvgNumEvents[length(reps$meanAvgNumEvents)]
      df$avgEventInSystemTime[i] <- reps$meanTotalQ[length(reps$meanTotalQ)]
    } else {
      avgEvent <- read.csv("./avgEventInSystemTime-values.dat", header=T)
      df$avgEventInSystemTime[i] <- avgEvent$avgEventSystemTimeMean[i]

      delay <- read.csv("./delay-values.dat", header=T)
      df$delay[i] <- delay$delayMean[i]

      avgNumEvents <- read.csv("./avgNumEvents-values.dat", header=T)
      df$numEvents[i] <- avgNumEvents$avgNumEventsMean[i]
    }
  } else {
    avgEvent <- read.csv("./avgEventInSystemTime-values.dat", header=T)
    df$avgEventInSystemTime[i] <- avgEvent$avgEventSystemTimeMean[i]
    
    delay <- read.csv("./delay-values.dat", header=T)
    df$delay[i] <- delay$delayMean[i]
    
    avgNumEvents <- read.csv("./avgNumEvents-values.dat", header=T)
    df$numEvents[i] <- avgNumEvents$avgNumEventsMean[i]
  }
}


write.csv(df, "simulationSummary.dat")

