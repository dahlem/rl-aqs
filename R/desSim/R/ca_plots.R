des.ca.ridge.spectrum.plot <- function(prefix, paths, start, end, ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-ridge-spectrum.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  lambdas <- read.csv(paste(prefix, "-ca-eigen.dat", sep=""), header=TRUE)
  data <- read.csv(paste(prefix, "-ridge_path-1.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))

  data <- data[data$incr < end,]
  data <- data[data$incr > start,]
  
  p <- ggplot()
  for (i in 0:(paths - 1)) {
    df <- data[data$path == i,];
    if (length(df$incr) > 0) {
      print(i)
      p <- p + geom_line(data=df, aes(x=incr, y=R))
    }
  }

  for (i in lambdas$lambda) {
    p <- p + geom_vline(xintercept=i, colour="red")
  }

  labels <- expression(lambda[1])
  for (i in 2:length(lambdas$theta)) {
    labels <- c(labels, bquote(lambda[.(i)]))
  }
  
  p <- p + scale_x_continuous("", breaks=lambdas$lambda, labels=labels)
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.ridge.x.plot <- function(prefix, path, paths, end, ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-ridge-x.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  data <- read.csv(paste(prefix, "-ridge_path-1.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  data <- data[data$incr < end,]
  df <- data[data$path == path,]
  labels <- data.frame(x=rep(0, paths-1), y=rep(0, paths-1), label=I(letters[1:(paths-1)]))

  p <- ggplot()

  for (i in 1:(paths - 1)) {
    p <- p + geom_line(data=df, aes_string(x="R", y=paste("x", i, sep="")), linetype=i)
    labels$x[i] <- max(df$R)
    labels$y[i] <- get.var(paste("x", i, sep=""), df)[which(df$R == labels$x[i])]
    labels$label[i] <- paste("x", i, sep="")
  }

  p <- p + geom_text(data=labels, aes(x=x, y=y, label=label), size=3, hjust=0)
  p <- p + scale_y_continuous("")
  p <- p + scale_linetype("Stable")
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.ridge.x.plot.2 <- function(prefix, path, paths, end, ps=TRUE, labels=c(expression(alpha), expression(lambda), expression(eta))) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-ridge-x.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  data <- read.csv(paste(prefix, "-ridge_path-1.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  data <- data[data$incr < end,]
  df <- data[data$path == path,]

  for (i in 1:(paths - 1)) {
    if (i == 1) {
      dfPrep <- data.frame(R=df$R, y=df[,(3+i)], x=rep(i, length(df$R)))
    } else {
      dfPrep <- rbind(dfPrep, data.frame(R=df$R, y=df[,(3+i)], x=rep(i, length(df$R))))
    }
  }

  p <- ggplot()
  p <- p + geom_line(data=dfPrep, aes(x=R, y=y, linetype=factor(x)))
  p <- p + scale_y_continuous("")
  p <- p + scale_linetype("Variable", labels=labels)
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
theme_white <- function() {
  theme_update(panel.background = theme_blank(),
               panel.grid.major = theme_blank(),
               panel.grid.minor = theme_blank())
}

des.ca.ridge.x.plot.3 <- function(prefix, path, paths, end, inlay=TRUE, mainScale=3, subScale=1, ps=TRUE, labels=c(expression(alpha), expression(lambda), expression(eta)),
                                  ncps=c(0.13, 0.075), breaks=c(0.2, 0.35, 0.5)) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-ridge-x.eps", sep=""), width=3.0, height=2.8, pointsize=8)
  }

  data <- read.csv(paste(prefix, "-ridge_path-", mainScale, ".dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  data <- data[data$incr < end,]
  df <- data[data$path == path,]

  for (i in 1:(paths - 1)) {
    if (i == 1) {
      dfPrep <- data.frame(R=df$R, y=df[,(3+i)], x=rep(i, length(df$R)))
    } else {
      dfPrep <- rbind(dfPrep, data.frame(R=df$R, y=df[,(3+i)], x=rep(i, length(df$R))))
    }
  }

  p <- ggplot()
  mp <- p + geom_line(data=dfPrep, aes(x=R, y=y, linetype=factor(x)))
  mp <- mp + scale_y_continuous("")
  mp <- mp + scale_x_continuous("R", breaks=breaks)
  mp <- mp + scale_linetype("Variable", breaks=unique(dfPrep$x), labels=labels)
  mp <- mp + opts(base_size=8)

  if (inlay) {
    sdata <- read.csv(paste(prefix, "-ridge_path-", subScale, ".dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
    sdata <- sdata[sdata$incr < end,]
    sdf <- sdata[sdata$path == path,]

    for (i in 1:(paths - 1)) {
      if (i == 1) {
        sdfPrep <- data.frame(R=sdf$R, y=sdf[,(3+i)], x=rep(i, length(sdf$R)))
      } else {
        sdfPrep <- rbind(sdfPrep, data.frame(R=sdf$R, y=sdf[,(3+i)], x=rep(i, length(sdf$R))))
      }
    }

    sp <- p + geom_line(data=sdfPrep, aes(x=R, y=y, linetype=factor(x)))
    sp <- sp + scale_y_continuous("")
    sp <- sp + scale_x_continuous("", breaks=NA)
    sp <- sp + scale_linetype("Variable", breaks=unique(sdfPrep$x), labels=labels)
    sp <- sp + opts(panel.border = theme_blank(), legend.position = "none")

    vp <- viewport(x=unit(ncps[1], "npc"), y=unit(ncps[2], "npc"), width = 0.4, height = 0.4, just = c("left", "bottom"))

    print(mp)
    theme_set(theme_bw(base_size=8))
    theme_white()
    print(sp, vp = vp)
    theme_set(theme_bw(base_size=8))
  } else {
    mp <- mp + theme_bw(base_size=8)
    print(mp)
  }
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.ridge.y.plot <- function(prefix, path, paths, end, ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-ridge-y.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  data <- read.csv(paste(prefix, "-ridge_path-1.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  data <- data[data$incr < end,]
  df <- data[data$path == path,]
  p <- ggplot(df)
  p <- p + geom_line(aes(x=R, y=y))

  p <- p + scale_y_continuous(expression(hat(y)))
  p <- p + theme_bw(base_size=8)

  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.ridge.y.group.plot <- function(prefix, paths, ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-ridge-y.eps", sep=""), width=2.8, height=2.8, pointsize=8)
  }

  data <- read.csv(paste(prefix, "-ridge_path-1.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  data$path <- data$path + 1
  p <- ggplot(data, aes(x=R, y=y, linetype=factor(path)))
  p <- p + geom_line()
  p <- p + scale_linetype("Path")
  p <- p + scale_y_continuous(expression(hat(y)))
  p <- p + theme_bw(base_size=8)

  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.plot <- function(prefix="2d-shdf", ps=TRUE) {
  if (ps) {
    des.postscript(paste(prefix, "-ca-model.eps", sep=""), width=3.0, height=2.8, pointsize=8)
  }

  ca <- read.table(paste(prefix, "-ca-model.dat", sep=""), sep=",", header=TRUE)
  eigen <- read.table(paste(prefix, "-ca-eigen.dat", sep=""), sep=",", header=TRUE)
  df <- data.frame(x=ca$x1, y=ca$x2, z=ca$y);

  isRidge <- FALSE;

  if ((eigen$theta[1] > 0) & (eigen$theta[2] > 0)) {
    isRidge <- FALSE;
  } else if ((eigen$theta[1] < 0) & (eigen$theta[2] < 0)) {
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
  p <- p + stat_contour(aes(colour = ..level..))
  p <- p + scale_y_continuous(substitute(x[I], list(I = 2)))
  p <- p + scale_x_continuous(substitute(x[I], list(I = 1)))
  p <- p + scale_colour("Elevation")

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
  p <- p + theme_bw(base_size=8)
  print(p)
  
  if (ps) {
    dev.off()
  }
}

