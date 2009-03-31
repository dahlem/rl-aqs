des.ca.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-ca-model.eps", sep=""), onefile=FALSE)
  }

  ca <- read.table(paste(prefix, "-ca-model.dat", sep=""), sep=",", header=TRUE)
  eigen <- read.table(paste(prefix, "-ca-eigen.dat", sep=""), sep=",", header=TRUE)
  df <- data.frame(x=ca$x1, y=ca$x2, z=ca$y);

  isRidge <- FALSE;

  if ((eigen$lambda[1] > 0) & (eigen$lambda[2] > 0)) {
    isRidge <- FALSE;
  } else if ((eigen$lambda[1] < 0) & (eigen$lambda[2] < 0)) {
    isRidge <- FALSE;
  } else {
    isRidge <- TRUE;
  }
  
  dim <- length(eigen$lambda)
  M <- cbind(eigen$M1, eigen$M2);
  MT <- t(M);

  if (!isRidge) {
    pS <- read.table(paste(prefix, "-ca-statP.dat", sep=""), sep=",", header=TRUE)
    z1Intercept <- (M[1,2] * pS$x2 + M[1,1] * pS$x1) / M[1,1];
    z2Intercept <- (M[2,1] * pS$x1 + M[2,2] * pS$x2) / M[2,2];
    z1Slope <- M[1,2] / M[1,1];
    z2Slope <- M[2,1] / M[2,2];
    
    minX <- min(ca$x1);
    minY <- min(ca$x2);
    maxX <- max(ca$x1);
    maxY <- max(ca$x2);

    ## calculate the coordinates for the transformed coord system
    ## z2
    ## end point
    x11 <- z1Intercept + maxY * z1Slope;
    x12 <- maxY;

    ## start point
    x112 <- minY + (pS$x2 - minY) / 2;
    x111 <- z1Intercept + x112 * z1Slope;

    ## z1
    ## end point
    x22 <- z2Intercept + maxX * z2Slope;
    x21 <- maxX;

    ## start point
    x221 <- minX + (pS$x1 - minX) / 2;
    x222 <- z2Intercept + x221 * z2Slope;

    ## if they are out-of bounds, adjust accordingly
    ## z2: 1. case: x11 > maxX
    ## z2: 2. case: x11 < minX
    if (x11 > maxX) {
      x11 <- maxX;
      x12 <- (x11 - z1Intercept) / z1Slope;
    } else if (x11 < minX) {
      x11 <- minX;
      x12 <- (x11 - z1Intercept) / z1Slope;
    }

    if (x111 > maxX) {
      x111 <- pS$x1 + (maxX - pS$x1) / 2;
      x112 <- (x111 - z1Intercept) / z1Slope;
    } else if (x111 < minX) {
      x111 <- minX + (pS$x1 - minX) / 2;
      x112 <- (x111 - z1Intercept) / z1Slope;
    }
    
    ## z1: 1. case: x22 > maxY
    ## z1: 2. case: x22 < minY
    if (x22 > maxY) {
      x22 <- maxY;
      x21 <- (x22 - z2Intercept) / z2Slope;
    } else if (x22 < minY) {
      x22 <- minY;
      x21 <- (x22 - z2Intercept) / z2Slope;
    }
    
    if (x222 > maxY) {
      x222 <- pS$x2 + (maxY - pS$x2) / 2;
      x221 <- (x222 - z2Intercept) / z2Slope;
    } else if (x222 < minY) {
      x222 <- minY + (pS$x2 - minY) / 2;
      x221 <- (x222 - z2Intercept) / z2Slope;
    }
  }
  
  p <- ggplot(df, aes(x=x, y=y, z=z))
  p <- p + stat_contour(aes(size = ..level..))
  p <- p + scale_y_continuous(substitute(x[I], list(I = 2)))
  p <- p + scale_x_continuous(substitute(x[I], list(I = 1)))
  p <- p + opts(title="Canonical Form of the Second-order Model")

  if (!isRidge) {
    pS <- read.table(paste(prefix, "-ca-statP.dat", sep=""), sep=",", header=TRUE)
    p <- p + geom_segment(x=pS$x1,y=minY,xend=pS$x1,yend=pS$x2,linetype=2,size=0.5,colour="gray40")
    p <- p + geom_text(x=pS$x1,y=minY,vjust=1.5,size=4,colour="gray40",aes(label="x1(S)"))
    p <- p + geom_segment(x=minX,y=pS$x2,xend=pS$x1,yend=pS$x2,linetype=2,size=0.5,colour="gray40")
    p <- p + geom_text(x=minX,y=pS$x2,vjust=1.5,size=4,colour="gray40",aes(label="x2(S)"))

    ## plot the transformed coordinate system
    p <- p + geom_segment(x=x111,y=x112,xend=x11,yend=x12,arrow=arrow(length=unit(0.2,"cm")))
    p <- p + geom_text(x=x11,y=x12,vjust=-1,size=4,aes(label="z2"))
    p <- p + geom_segment(x=x221,y=x222,xend=x21,yend=x22,arrow=arrow(length=unit(0.2,"cm")))
    p <- p + geom_text(x=x21,y=x22,hjust=-1,size=4,aes(label="z1"))
  }
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.evo.plot <- function(prefix="2d-shdf", betas=1, thetas=2) {
  des.kriging.mcmc.posterior.plot(prefix)

  for (i in 1:betas) {
    des.kriging.mcmc.beta.plot(prefix, idx=as.character(i))
  }
  des.kriging.mcmc.sigma.plot(prefix)
  for (i in 1:betas) {
    des.kriging.mcmc.theta.plot(prefix, idx=as.character(i))
  }
}


des.nonst.kriging.mcmc.evo.plot <- function(prefix="2d-shdf", betas=1, etasr=1, etasc=1) {
  for (i in 1:betas) {
    des.kriging.mcmc.beta.plot(prefix, "-nonst", idx=as.character(i))
  }
  des.kriging.mcmc.sigma.plot(prefix, "-nonst")
  for (i in 1:etasr) {
    for (j in 1:etasc) {
      des.kriging.mcmc.eta.plot(prefix, i, j)
    }
  }
}


des.kriging.mcmc.posterior.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-posterior.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-posterior.dat", sep=""), header=TRUE)
  df <- data.frame(x=1:length(chain$posterior), y=chain$posterior)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(paste("-log ", p(theta))))
  p <- p + scale_x_continuous("")
  p <- p + opts(title=expression(paste("Evolution of ", "-log ", p(theta))))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.beta.plot <- function(prefix="2d-shdf", nonst="", idx="1", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain", nonst ,"-beta_", idx , ".eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain", nonst ,"-beta_", idx , ".dat", sep=""), header=TRUE)
  df <- data.frame(x=1:length(chain$beta), y=chain$beta)

  i <- as.numeric(idx)
  ytitle <- bquote(paste(beta[.(i)]))
  title <- bquote(paste("Evolution of ", beta[.(i)]))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.sigma.plot <- function(prefix="2d-shdf", nonst="", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain", nonst ,"-sigma.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain", nonst ,"-sigma.dat", sep=""), header=TRUE)
  df <- data.frame(x=1:length(chain$sigma), y=chain$sigma)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(sigma^2))
  p <- p + scale_x_continuous("")
  p <- p + opts(title=expression(paste("Evolution of ", sigma^2)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.theta.plot <- function(prefix="2d-shdf", idx="1", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-theta", idx, ".eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-theta", idx, ".dat", sep=""),
                      header=TRUE, col.names=c("theta"))
  df <- data.frame(x=1:length(chain$theta), y=chain$theta)

  i <- as.numeric(idx)
  ytitle <- bquote(paste(theta[.(i)]))
  title <- bquote(paste("Evolution of ", theta[.(i)]))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.eta.plot <- function(prefix="2d-shdf", dim="1", idx="1", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-nonst-chain-eta", dim, "_", idx, ".eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-nonst-chain-eta", dim, "_", idx, ".dat", sep=""),
                      header=TRUE, col.names=c("eta"))
  df <- data.frame(x=1:length(chain$eta), y=chain$eta)
  i <- as.numeric(idx)
  j <- as.numeric(dim)
  ytitle <- bquote(paste(eta[.(i)]^(.(j))))
  title <- bquote(paste("Evolution of ", eta[.(i)]^(.(j))))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
  
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.mean.plot <- function(prefix="2d-shdf", betas=1, thetas=1) {
  des.kriging.mcmc.posterior.mean.plot(prefix)

  for (i in 1:betas) {
    des.kriging.mcmc.beta.mean.plot(prefix, idx=as.character(i))
  }
  des.kriging.mcmc.sigma.mean.plot(prefix)
  for (i in 1:thetas) {
    des.kriging.mcmc.theta.mean.plot(prefix, idx=as.character(i))
  }
}


des.nonst.kriging.mcmc.mean.plot <- function(prefix="2d-shdf", betas=1, etasr=1, etasc=1) {
  des.kriging.mcmc.posterior.mean.plot(prefix)

  for (i in 1:betas) {
    des.kriging.mcmc.beta.mean.plot(prefix, "-nonst", idx=as.character(i))
  }
  des.kriging.mcmc.sigma.mean.plot(prefix, "-nonst")
  for (i in 1:etasr) {
    for (j in 1:etasc) {
      des.kriging.mcmc.eta.mean.plot(prefix, i, j)
    }
  }
}


des.kriging.mcmc.posterior.mean.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (file.exists(paste(prefix, "-chain-posterior-mean.dat", sep=""))) {
    if (ps) {
      postscript(paste(prefix, "-chain-posterior-mean.eps", sep=""), onefile=FALSE)
    }

    chain <- read.table(paste(prefix, "-chain-posterior-mean.dat", sep=""), header=TRUE)
    df <- data.frame(x=1:length(chain$mean_posterior), y=chain$mean_posterior)
    p <- ggplot(df, aes(x=x, y=y))
    p <- p + layer(geom = "line")
    p <- p + scale_y_continuous(expression(paste("-log", p(theta))))
    p <- p + scale_x_continuous("")
    p <- p + opts(title=expression(paste("Equilibration of ", "-log", p(theta))))
    p <- p + theme_bw()
    print(p)
    
    if (ps) {
      dev.off()
    }
  }
}


des.kriging.mcmc.beta.mean.plot <- function(prefix="2d-shdf", nonst="", idx="1", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain", nonst , "-beta_", idx , "-mean.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain", nonst , "-beta_", idx , "-mean.dat", sep=""),
                      header=TRUE, col.names=("mean_beta"))
  df <- data.frame(x=1:length(chain$mean_beta), y=chain$mean_beta)

  i <- as.numeric(idx)
  ytitle <- bquote(paste(bar(beta)[.(i)]))
  title <- bquote(paste("Equilibration of ", bar(beta)[.(i)]))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.sigma.mean.plot <- function(prefix="2d-shdf", nonst="", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain", nonst, "-sigma-mean.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain", nonst, "-sigma-mean.dat", sep=""), header=TRUE)
  df <- data.frame(x=1:length(chain$mean_sigma), y=chain$mean_sigma)
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(expression(bar(sigma)^2))
  p <- p + scale_x_continuous("")
  p <- p + opts(title=expression(paste("Equilibration of ", bar(sigma)^2)))
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

  chain <- read.table(paste(prefix, "-chain-theta_", idx, "-mean.dat", sep=""),
                      header=TRUE, col.names=c("theta"))
  df <- data.frame(x=1:length(chain$theta), y=chain$theta)
  i <- as.numeric(idx)
  ytitle <- bquote(paste(bar(theta)[.(i)]))
  title <- bquote(paste("Equilibration of ", bar(theta)[.(i)]))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
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

  ytitle <- bquote(paste(bar(eta)[.(i)]^(.(j))))
  title <- bquote(paste("Equilibration of ", bar(eta)[.(i)]^(.(j))))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous("")
  p <- p + opts(title=title)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.autocorr.plots <- function(prefix="2d-shdf", betas=1, thetas=1) {
  mcmcanalysis <- read.csv(paste(prefix, "-autocorr.dat", sep=""), header=TRUE)

  for (i in 1:betas) {
    des.kriging.mcmc.autocorr.plot(prefix, var=paste("beta_", i, sep=""), wopt=mcmcanalysis$wopt[i], ps=TRUE)
  }
  
  des.kriging.mcmc.autocorr.plot(prefix, var="sigma", wopt=mcmcanalysis$wopt[betas+1], ps=TRUE)
  for (i in 1:thetas) {
    des.kriging.mcmc.autocorr.plot(prefix, var=paste("theta", i, sep=""), wopt=mcmcanalysis$wopt[betas+i+1], ps=TRUE)
  }

  for (i in 1:betas) {
    des.kriging.mcmc.tauintvsw.plot(prefix, var=paste("beta_", i, sep=""), wopt=mcmcanalysis$wopt[i], taui=mcmcanalysis$tauint[i], ps=TRUE)
  }
  des.kriging.mcmc.tauintvsw.plot(prefix, var="sigma", wopt=mcmcanalysis$wopt[2], taui=mcmcanalysis$tauint[betas+1], ps=TRUE)
  for (i in 1:thetas) {
    des.kriging.mcmc.tauintvsw.plot(prefix, var=paste("theta", i, sep=""), wopt=mcmcanalysis$wopt[betas+i+1], taui=mcmcanalysis$tauint[betas+i+1], ps=TRUE)
  }
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
  } else if (var == "beta_1") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", beta[1])))
  } else if (var == "beta_2") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", beta[2])))
  } else if (var == "beta_3") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", beta[3])))
  } else if (var == "beta_4") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", beta[4])))
  } else if (var == "sigma") {
    p <- p + opts(title=expression(paste("Normalised Autocorrelation of ", sigma^2)))
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


des.kriging.nonst.mcmc.autocorr.plot <- function(prefix="2d-shdf", i, j, wopt, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-nonst-normalised-autocorr-mass-eta", i, "_", j, ".eps", sep=""),
               onefile=FALSE)
  }

  norCorr <- read.csv(paste(prefix, "-nonst-normalised-autocorr-mass-eta", i, "_", j, ".dat", sep=""),
                      header=TRUE)
  
  df <- data.frame(tauint=norCorr$tauint,
                   rho=norCorr$rho,
                   min=norCorr$rho-norCorr$drho,
                   max=norCorr$rho+norCorr$drho)

  limits <- aes(max = max, min = min)
  title <- bquote(paste("Normalised Autocorrelation of ", eta[.(i)]^(.(j))))

  p <- ggplot(df, aes(x=tauint, y=rho))
  p <- p + layer(geom = "line")
  p <- p + geom_errorbar(limits, width=0.5)
  p <- p + scale_y_continuous(expression(rho), breaks=c(-0.5, 0, 0.5, 1))
  p <- p + scale_x_continuous("W")
  p <- p + geom_vline(intercept=wopt, colour="red")
  p <- p + theme_bw()
  p <- p + opts(title=title)

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
  } else if (var == "beta_1") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", beta[1])))
  } else if (var == "beta_2") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", beta[2])))
  } else if (var == "beta_3") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", beta[3])))
  } else if (var == "beta_4") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", beta[4])))
  } else if (var == "sigma") {
    p <- p + opts(title=expression(paste(tau[int], " with Statistical Errors of ", sigma^2)))
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


des.kriging.nonst.mcmc.tauintvsw.plot <- function(prefix="2d-shdf", i, j, wopt, taui, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-nonst-tauint_vs_w-eta", i, "_", j, ".eps", sep=""), onefile=FALSE)
  }

  csv <- read.csv(paste(prefix, "-nonst-tauint_vs_w-eta", i, "_", j, ".dat", sep=""), header=TRUE)
  df <- data.frame(w=csv$W,
                   tauint=csv$tauint,
                   error=csv$error,
                   min=csv$tauint-csv$error,
                   max=csv$tauint+csv$error)

  limits <- aes(max = max, min = min)
  title <- bquote(paste(tau[int], " with Statistical Errors of ", eta[.(i)]^(.(j))))

  p <- ggplot(df, aes(x=w, y=tauint))
  p <- p + layer(geom = "line")
  p <- p + geom_errorbar(limits, width=0.5)
  p <- p + scale_x_continuous("W")
  p <- p + scale_y_continuous(expression(tau[int]))
  p <- p + geom_hline(intercept=taui, linetype=2, colour="blue")
  p <- p + geom_vline(intercept=wopt, colour="red")
  p <- p + theme_bw()
  p <- p + opts(title=title)

  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.likeli.plot <- function(prefix="2d-shdf", betas=1, thetas=2, ps=TRUE) {
  for (i in 1:betas) {
    des.kriging.mcmc.beta.likeli.plot(prefix, idx=as.character(i), ps);
  }
  des.kriging.mcmc.sigma.likeli.plot(prefix, ps);
  for (i in 1:thetas) {
    des.kriging.mcmc.theta.likeli.plot(prefix, idx=as.character(i), ps);
  }
}


des.kriging.mcmc.beta.likeli.plot <- function(prefix="2d-shdf", idx="1", ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-beta_", idx, "-likelihood.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-beta_", idx , "-likelihood.dat", sep=""), sep=",", header=TRUE)
  df <- data.frame(y=chain$likelihood_beta/sum(chain$likelihood_beta),x=chain$beta)

  i <- as.numeric(idx)
  ytitle <- bquote(paste("L(", beta[.(i)], "|", y, ",", sigma^2, ",", theta, ")"))

  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous(expression(beta[.(i)]))
  p <- p + opts(title=expression(paste("Likelihood over ", beta[.(i)])))
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




des.kriging.nonst.mcmc.eta.likeli.plot <- function(prefix="2d-shdf", i, j, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-chain-nonst-eta", i, "_", j, "-likelihood.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-nonst-eta", i, "_", j, "-likelihood.dat", sep=""), header=TRUE,
                      col.names=c("likelihood_eta", "eta"), sep=",")
  df <- data.frame(x=chain$eta,y=(chain$likelihood_eta/sum(chain$likelihood_eta)))
  ytitle <- bquote(paste("L(", eta[.(i)]^(.(j)), ":", y, ",", beta, ",", sigma^2, ")"))
  xtitle <- bquote(eta[.(i)]^(.(j)))
  title <- bquote(paste("Likelihood over ", eta[.(i)]^(.(j))))
  
  p <- ggplot(df, aes(x=x, y=y))
  p <- p + layer(geom = "line")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous(xtitle)
  p <- p + opts(title=title)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}


des.kriging.mcmc.marginal.plot <- function(prefix="2d-shdf", betas, thetas, ps=TRUE) {

  for (i in 1:betas) {
    des.kriging.mcmc.beta.marginal.plot(prefix, idx=as.character(i), ps, 3);
  }
  des.kriging.mcmc.sigma.marginal.plot(prefix, ps, 3);
  for (i in 1:thetas) {
    des.kriging.mcmc.theta.marginal.plot(prefix, idx=i, ps, 1);
  }
}


des.kriging.mcmc.beta.marginal.plot <- function(prefix="2d-shdf", idx="1", ps=TRUE, adj=1) {
  if (ps) {
    postscript(paste(prefix, "-chain-beta_", idx , "-sampled.eps", sep=""), onefile=FALSE)
  }

  chain <- read.table(paste(prefix, "-chain-beta_", idx, "-sampled.dat", sep=""), header=TRUE)
  df <- data.frame(x=chain$sampled_beta)

  i <- as.numeric(idx)
  ytitle <- bquote(paste("p(", beta[.(i)], ")"))

  p <- ggplot(df, aes(x=x))
  p <- p + geom_histogram(aes(y= ..density..))
  p <- p + geom_density(aes(y= ..density..), kernel="gaussian", adjust=adj, fill=NA, colour="black")
  p <- p + scale_y_continuous(ytitle)
  p <- p + scale_x_continuous(expression(beta[.(i)]))
  p <- p + opts(title=expression(paste("Marginal Probability of ", beta[.(i)])))
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
