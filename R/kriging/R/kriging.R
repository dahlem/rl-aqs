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
## Keywords: kriging
## Created: 11.05.2009

library(spBayes)
library(MBA)
library(fields)
library(geoR)
library(classInt)

prefix <- "rand-W-"

data <- read.table(paste(prefix, "X.dat", sep=""), header=TRUE)
Y <- data[,"y"]
coords <- as.matrix(data[,c("x1", "x2")])


## explore the data
col.br <- colorRampPalette(c("blue", "cyan", "yellow", "red"))
col.pal <- col.br(5)

quant <- classIntervals(Y, n=5, style="quantile")
fisher <- classIntervals(Y, n=5, style="fisher")
kmeans <- classIntervals(Y, n=5, style="kmeans")
bclust <- classIntervals(Y, n=5, style="bclust")

par(mfrow=c(2,2))
plot(quant, pal=col.pal, xlab="Y", main="Quantile")
plot(fisher, pal=col.pal, xlab="Y", main="Fisher")
plot(kmeans, pal=col.pal, xlab="Y", main="Kmeans")
plot(bclust, pal=col.pal, xlab="Y", main="Bclust")

quant.col <- findColours(quant, col.pal)
fisher.col <- findColours(fisher, col.pal) 

par(mfrow=c(1,2))
plot(coords, col=quant.col, pch=19, cex=0.5, main="Quantile", xlab="Easting (m)", ylab="Northing (m)")
legend("topleft", fill=attr(quant.col, "palette"), legend=names(attr(quant.col, "table")), bty="n")

plot(coords, col=fisher.col, pch=19, cex=0.5, main="Bclust", xlab="Easting (m)", ylab="Northing (m)")
legend("topleft", fill=attr(fisher.col, "palette"), legend=names(attr(fisher.col, "table")), bty="n")

x.res <- 100
y.res <- 100

surf <- mba.surf(cbind(coords, Y), no.X=x.res, no.Y=y.res, h=5, m=2, extend=FALSE)$xyz.est
image.plot(surf, xaxs = "r", yaxs = "r", xlab="Easting (m)", ylab="Northing (m)", col=col.br(25))

drape.plot(surf[[1]], surf[[2]], surf[[3]], col=col.br(150), theta=225, phi=50, border=FALSE,
           add.legend=FALSE, xlab="Easting (m)", ylab="Northing (m)", zlab="Y")

image.plot(zlim=range(surf[[3]], na.rm=TRUE), legend.only=TRUE, horizontal=FALSE)

lm.Y <- lm(Y~x1+x2, data=data)
summary(lm.Y)

Y.resid <- resid(lm.Y)



## variogram analysis
## not here


## specify the priors
phi.prior <- prior(dist="UNIF", a=0.06, b=3)
sigmasq.prior <- prior(dist="IG", shape=2, scale=5)
nu.prior <- prior(dist="UNIF", a=0.01, b=2)
beta.prior <- prior(dist="NORMAL", mu=rep(0, 3), precision=diag(0.0001, 3))

## specify the update control
var.update.control <- list("K"=list(sample.order=1, starting=1, tuning=0.25, prior=sigmasq.prior),
                           "phi"=list(sample.order=1, starting=0.5, tuning=0.25, prior=phi.prior),
                           "nu"=list(sample.order=1, starting=0.1, tuning=0.25, prior=nu.prior),
                           )

## specify the gibbs steps for the betas
beta.update.control <- list(update="MH", prior=beta.prior)

run.control <- list("n.samples"=10000, "sp.effects"=TRUE, DIC=TRUE, DIC.start=501)

## the model depends on two variables x1 and x2
model <- y ~ x1 + x2

Fit.m <- spGGT(formula=model, data=data, run.control=run.control,
               coords=coords, var.update.control=var.update.control,
               beta.update.control=beta.update.control,
               cov.model="matern")

par(mfrow=c(3,2))
plot(Fit.m$p.samples[,1:6], auto.layout=TRUE, density=FALSE)

par(mfrow=c(2,2))
effective.range <- 3/Fit.m$p.samples[,6]
plot(mcmc(cbind(Fit.m$p.samples[,1:2],effective.range)), auto.layout=TRUE, density=FALSE)

n.samples <- nrow(Fit.m$p.samples)
burn.in <- as.integer(0.5*n.samples)
p.samples <- Fit.m$p.samples[burn.in:n.samples,]

m.surf <- mba.surf(cbind(coords, rowMeans(Fit.m$sp.effects)), no.X=100, no.Y=100, extend=TRUE)$xyz.est
image(m.surf, xaxs="r", yaxs="r", main="Y random spatial effects")
points(coords, pch=19)


w.hat.mu <- apply(Fit.m$sp.effects[,burn.in:ncol(Fit.m$sp.effects)],1,mean)
w.hat.sd <- apply(Fit.m$sp.effects[,burn.in:ncol(Fit.m$sp.effects)],1,sd)


par(mfrow=c(1,2))
surf <- mba.surf(cbind(coords, Y.resid), no.X=x.res, no.Y=y.res, extend=FALSE)$xyz.est
z.lim <- range(surf[[3]], na.rm=TRUE)
image.plot(surf, xaxs = "r", yaxs = "r", zlim=z.lim, main="LM residuals")

surf <- mba.surf(cbind(coords, w.hat.mu), no.X=x.res, no.Y=y.res, extend=FALSE)$xyz.est
image.plot(surf, xaxs = "r", yaxs = "r", main="Mean spatial effects")

par(mfrow=c(1,2))
surf <- mba.surf(cbind(coords, Y.resid), no.X=x.res, no.Y=y.res, extend=FALSE)$xyz.est
image.plot(surf, xaxs = "r", yaxs = "r", main="LM residuals")

surf <- mba.surf(cbind(coords, w.hat.sd), no.X=x.res, no.Y=y.res, extend=FALSE)$xyz.est
image.plot(surf, xaxs = "r", yaxs = "r", main="SD spatial effects")


X <- Fit.m$X
beta <- t(p.samples[,1:Fit.m$p])
w <- Fit.m$sp.effects[,burn.in:ncol(Fit.m$sp.effects)]


## prediction
x1.range <- range(coords[,1])
x2.range <- range(coords[,2])

pred.coords <- expand.grid(seq(1.01,1.1,length=50), seq(1.0,1.1,length=50))
pred.covars <- as.matrix(rep(3, nrow(pred.coords)))
