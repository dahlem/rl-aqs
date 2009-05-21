des.ca.ridge.spectrum.plot <- function(prefix, paths, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-ca-ridge-spectrum.eps", sep=""), onefile=FALSE)
  }

  lambdas <- read.csv(paste(prefix, "-ca-eigen.dat", sep=""), header=TRUE)
  data <- read.csv(paste(prefix, "-ridge_path.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  p <- ggplot()

  for (i in 0:(paths - 1)) {
    df <- data[data$path == i,];
    p <- p + geom_line(data=df, aes(x=incr, y=R))
  }

  for (i in lambdas$lambda) {
    p <- p + geom_vline(xintercept=i, colour="red")
  }

  labels <- expression(lambda[1])
  for (i in 2:length(lambdas$lambda)) {
    labels <- c(labels, bquote(lambda[.(i)]))
  }
  
  p <- p + scale_x_continuous("", breaks=lambdas$lambda,
                              labels=labels)
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.ridge.x.plot <- function(prefix, path, paths, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-ca-ridge-x.eps", sep=""), onefile=FALSE)
  }

  data <- read.csv(paste(prefix, "-ridge_path.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  df <- data[data$path == path,]
  labels <- data.frame(x=rep(0, paths-1), y=rep(0, paths-1), label=I(letters[1:(paths-1)]))
  p <- ggplot()

  for (i in 1:(paths - 1)) {
    p <- p + geom_line(data=df, aes_string(x="R", y=paste("x", i, sep="")), linetype=i)
    labels$x[i] <- max(df$R)
    labels$y[i] <- get.var(paste("x", i, sep=""), df)[which(df$R == maxX)]
    labels$label[i] <- paste("x", i, sep="")
  }

  p <- p + geom_text(data=labels, aes(x=x, y=y, label=label), vjust=1.5)
  p <- p + scale_y_continuous("")
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
des.ca.ridge.maxY.plot <- function(prefix, path, ps=TRUE) {
  if (ps) {
    postscript(paste(prefix, "-ca-ridge-maxY.eps", sep=""), onefile=FALSE)
  }

  p <- ggplot()

  data <- read.csv(paste(prefix, "-ridge_path.dat", sep=""), header=FALSE, col.names=c("incr", "R", "y", paste("x", 1:(paths - 1), sep=""), "path"))
  df <- data[data$path == path,]
  p <- p + geom_line(data=df, aes(x=R, y=y))

  p <- p + scale_y_continuous(expression(hat(y)))
  p <- p + theme_bw()
  print(p)
  
  if (ps) {
    dev.off()
  }
}

  
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

