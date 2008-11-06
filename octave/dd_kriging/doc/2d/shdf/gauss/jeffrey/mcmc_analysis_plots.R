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
  p <- p + opts(title=expression(paste("Equilibration of ", bar(beta))))
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
  p <- p + opts(title=expression(paste("Equilibration of ", bar(sigma))))
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
  if (idx == "1") {
    p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[1]))))
  } else if (idx == "2") {
    p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[2]))))
  } else if (idx == "3") {
    p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[3]))))
  } else if (idx == "4") {
    p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[4]))))
  } else if (idx == "5") {
    p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[5]))))
  } else if (idx == "6") {
    p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[6]))))
  }
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.eta.mean.plot <- function(prefix="2d-shdf", i, j, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-nonst-chain-eta", i, "_", j, "-mean.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-nonst-chain-eta", i, "_", j, "-mean.dat", sep=""),
                      header=TRUE, col.names=c("eta"))
  df <- data.frame(x=1:length(chain$eta), y=chain$eta)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(substitute(eta[I]^J, list(I = i, J = j)))
  p <- p + scale_x_continuous("")
##   if (idx == "1") {
##     p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[1]))))
##   } else if (idx == "2") {
##     p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[2]))))
##   } else if (idx == "3") {
##     p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[3]))))
##   } else if (idx == "4") {
##     p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[4]))))
##   } else if (idx == "5") {
##     p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[5]))))
##   } else if (idx == "6") {
##     p <- p + opts(title=expression(paste("Equilibration of ", bar(theta[6]))))
##   }
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.autocorr.plots <- function(prefix="2d-shdf") {
  mcmcanalysis <- read.csv(paste(prefix, "-autocorr.dat", sep=""), header=TRUE)
  des.kriging.mcmc.autocorr.plot(prefix, var="beta", wopt=mcmcanalysis$wopt[1], ps=TRUE)
  des.kriging.mcmc.autocorr.plot(prefix, var="sigma", wopt=mcmcanalysis$wopt[2], ps=TRUE)
  des.kriging.mcmc.autocorr.plot(prefix, var="theta1", wopt=mcmcanalysis$wopt[3], ps=TRUE)
  des.kriging.mcmc.autocorr.plot(prefix, var="theta2", wopt=mcmcanalysis$wopt[4], ps=TRUE)

  des.kriging.mcmc.tauintvsw.plot(prefix, var="beta", wopt=mcmcanalysis$wopt[1], taui=mcmcanalysis$tauint[1], ps=TRUE)
  des.kriging.mcmc.tauintvsw.plot(prefix, var="sigma", wopt=mcmcanalysis$wopt[2], taui=mcmcanalysis$tauint[2], ps=TRUE)
  des.kriging.mcmc.tauintvsw.plot(prefix, var="theta1", wopt=mcmcanalysis$wopt[3], taui=mcmcanalysis$tauint[3], ps=TRUE)
  des.kriging.mcmc.tauintvsw.plot(prefix, var="theta2", wopt=mcmcanalysis$wopt[4], taui=mcmcanalysis$tauint[4], ps=TRUE)
}


des.kriging.mcmc.autocorr.plot <- function(prefix="2d-shdf", var="beta", wopt, ps=TRUE) {
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
  p <- p + scale_y_continuous(expression(rho), breaks=c(-0.5, 0, 0.5, 1))
  p <- p + scale_x_continuous("W")
  p <- p + geom_vline(intercept=wopt, colour="red")
  p <- p + theme_bw()
  
  if (var == "beta") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", beta)))
  } else if (var == "sigma") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", sigma)))
  } else if (var == "theta1") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", theta[1])))
  } else if (var == "theta2") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", theta[2])))
  } else if (var == "theta3") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", theta[3])))
  } else if (var == "theta4") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", theta[4])))
  } else if (var == "theta5") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", theta[5])))
  } else if (var == "theta6") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", theta[6])))
  }

  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.tauintvsw.plot <- function(prefix="2d-shdf", var="beta", wopt, taui, ps=TRUE) {
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
  p <- p + geom_hline(intercept=taui, linetype=2, colour="blue")
  p <- p + geom_vline(intercept=wopt, colour="red")
  p <- p + theme_bw()

  if (var == "beta") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", beta)))
  } else if (var == "sigma") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", sigma)))
  } else if (var == "theta1") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", theta[1])))
  } else if (var == "theta2") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", theta[2])))
  } else if (var == "theta3") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", theta[3])))
  } else if (var == "theta4") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", theta[4])))
  } else if (var == "theta5") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", theta[5])))
  } else if (var == "theta6") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", theta[6])))
  }

  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.likeli.plot <- function(prefix="2d-shdf", ps=TRUE) {
  des.kriging.mcmc.beta.likeli.plot(prefix, ps);
  des.kriging.mcmc.sigma.likeli.plot(prefix, ps);
  des.kriging.mcmc.theta.likeli.plot(prefix, idx=1, ps);
  des.kriging.mcmc.theta.likeli.plot(prefix, idx=2, ps);
}


des.kriging.mcmc.beta.likeli.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-beta-likelihood.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-beta-likelihood.dat", sep=""), sep=",", header=TRUE)
  df <- data.frame(y=chain$likelihood_beta/sum(chain$likelihood_beta),x=chain$beta)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(paste("L(", beta, "|", y, ",", sigma^2, ",", theta, ")")))
  p <- p + scale_x_continuous(expression(beta))
  p <- p + opts(title=expression(paste("Likelihood over ", beta)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.sigma.likeli.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-sigma-likelihood.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-sigma-likelihood.dat", sep=""), sep=",", header=TRUE)
  df <- data.frame(y=chain$likelihood_sigma/sum(chain$likelihood_sigma), x=chain$sigma)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(paste("L(", sigma^2, "|", y, ",", beta, ",", theta, ")")))
  p <- p + scale_x_continuous(expression(sigma^2))
  p <- p + opts(title=expression(paste("Likelihood over ", sigma^2)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.theta.likeli.plot <- function(prefix="2d-shdf", idx=1, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-theta", idx, "-likelihood.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-theta", idx, "-likelihood.dat", sep=""), header=TRUE,
                      col.names=c("likelihood_theta", "theta"), sep=",")
  df <- data.frame(x=chain$theta,y=(chain$likelihood_theta/sum(chain$likelihood_theta)))
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(substitute(paste("L(", theta[I], "|", y, ",", beta, ",", sigma^2, ")"), list(I = idx)))
  p <- p + scale_x_continuous(substitute(theta[I], list(I = idx)))
  p <- p + opts(title=substitute(paste("Likelihood over ", theta[I]), list(I = idx)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.marginal.plot <- function(prefix="2d-shdf", ps=TRUE) {
  des.kriging.mcmc.beta.marginal.plot(prefix, ps, 3);
  des.kriging.mcmc.sigma.marginal.plot(prefix, ps, 3);
  des.kriging.mcmc.theta.marginal.plot(prefix, idx=1, ps, 1);
  des.kriging.mcmc.theta.marginal.plot(prefix, idx=2, ps, 1);
}


des.kriging.mcmc.beta.marginal.plot <- function(prefix="2d-shdf", ps=TRUE, adj=1) {
  if (ps) {
    postscript(paste(prefix, "-chain-beta-sampled.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-beta-sampled.dat", sep=""), header=TRUE)
  df <- data.frame(x=chain$sampled_beta)
  p <- ggplot(df, aes(x=x))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=adj, fill=NA, colour="black")
  p <- p + scale_y_continuous(expression(paste("p(", beta, ")")))
  p <- p + scale_x_continuous(expression(beta))
  p <- p + opts(title=expression(paste("Marginal Probability of ", beta)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.sigma.marginal.plot <- function(prefix="2d-shdf", ps=TRUE, adj=1) {
  if (ps) {
    postscript(paste(prefix, "-chain-sigma-sampled.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-sigma-sampled.dat", sep=""), header=TRUE)
  df <- data.frame(x=chain$sampled_sigma)
  p <- ggplot(df, aes(x=x))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=adj, fill=NA, colour="black")
  p <- p + scale_y_continuous(expression(paste("p(", sigma^2, ")")))
  p <- p + scale_x_continuous(expression(sigma^2))
  p <- p + opts(title=expression(paste("Marginal Probability of ", sigma^2)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.theta.marginal.plot <- function(prefix="2d-shdf", ps=TRUE, idx=1, adj=1) {
  if (ps) {
    postscript(paste(prefix, "-chain-theta", idx, "-sampled.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-theta", idx, "-sampled.dat", sep=""), header=TRUE,
                      col.names=c("sampled_theta"))
  df <- data.frame(x=chain$sampled_theta)
  p <- ggplot(df, aes(x=x))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=adj, fill=NA, colour="black")
  p <- p + scale_y_continuous(substitute(paste("p(", theta[I], ")"), list(I = idx)))
  p <- p + scale_x_continuous(substitute(theta[I], list(I = idx)))
  p <- p + opts(title=substitute(paste("Marginal Probability of ", theta[I]), list(I = idx)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}
