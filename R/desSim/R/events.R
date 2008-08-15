## Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: analysis, discrete-event simulation, events, inter-arrival times
## Created: 15.07.2008

des.events.global.interarrival <- function(events) {
  interArrivalTimes = c()
  
  for (e in 2:length(events$time)) {
    interArrivalTime <- events$time[e] - events$time[e-1]
    interArrivalTimes <- append(interArrivalTimes, interArrivalTime)
  }

  return (data.frame(interArrivalTimes))
}

des.events.interarrival.plot <- function(interArrivalEvents) {
  m <- ggplot(interArrivalEvents, aes(x=interArrivalTimes))
  m + geom_density()
}

des.events.interarrival.density.plot <- function(interArrivalEvents) {
  dens <- density(interArrivalEvents$interArrivalTimes, kernel="gaussian")
  plot(dens)
}
