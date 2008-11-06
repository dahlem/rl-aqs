

des.kriging.mcmc.autocorr.plot <- function(prefix="2d-shdf", var="beta", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-normalised-autocorr-mass-", var, ".eps", sep=""),
               onefile=FALSE)
  }

  norCorr <- read.csv(paste(prefix, "-normalised-autocorr-mass-", var, ".dat", sep=""),
                      header=TRUE)
  df <- data.frame(tauint=norCorr$tauint,
                   rho=norCorr$rho,
                   min=norCorr$rho-norCorr$drho,
                   max=norCorr$rho+norCorr$drho)

  limits <- aes(max = max, min = min)
  p <- ggplot(df, aes(x=tauint, y=rho))
  p <- p + layer(geom = "line")
  p <- p + geom_errorbar(limits, width=0.5)
  p <- p + scale_x_continuous(expression(
      paste(tau[int], " with statistical errors of mass")))
  p <- p + scale_y_continuous(expression(rho), breaks=c(-0.5, 0, 0.5, 1))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.tauintvsw.plot <- function(prefix="2d-shdf", var="beta", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-tauint_vs_w-", var, ".eps", sep=""), onefile=FALSE)
  }

  csv <- read.csv(paste(prefix, "-tauint_vs_w-", var, ".dat", sep=""), header=TRUE)
  df <- data.frame(w=csv$W,
                   tauint=csv$tauint,
                   error=csv$error,
                   min=csv$tauint-csv$error,
                   max=csv$tauint+csv$error)

  limits <- aes(max = max, min = min)
  p <- ggplot(df, aes(x=w, y=tauint))
  p <- p + layer(geom = "line")
  p <- p + geom_errorbar(limits, width=0.5)
  p <- p + scale_x_continuous("W")
  p <- p + scale_y_continuous(expression(tau[int]))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.mean.plot <- function(prefix="2d-shdf") {
  des.kriging.mcmc.beta.mean.plot(prefix)
  des.kriging.mcmc.sigma.mean.plot(prefix)
  des.kriging.mcmc.theta.mean.plot(prefix, idx="1")
  des.kriging.mcmc.theta.mean.plot(prefix, idx="2")
}


des.kriging.mcmc.beta.mean.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-beta-mean.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-beta-mean.dat", sep=""), header=TRUE)
  df <- data.frame(x=1:length(chain$mean_beta), y=chain$mean_beta)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(beta))
  p <- p + scale_x_continuous("")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.sigma.mean.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-sigma-mean.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-sigma-mean.dat", sep=""), header=TRUE)
  df <- data.frame(x=1:length(chain$mean_sigma), y=chain$mean_sigma)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(sigma))
  p <- p + scale_x_continuous("")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.theta.mean.plot <- function(prefix="2d-shdf", idx="1", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-theta", idx, "-mean.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-theta", idx, "-mean.dat", sep=""),
                      header=TRUE, col.names=c("theta"))
  df <- data.frame(x=1:length(chain$theta), y=chain$theta)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(substitute(theta[I], list(I = idx)))
  p <- p + scale_x_continuous("")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}
