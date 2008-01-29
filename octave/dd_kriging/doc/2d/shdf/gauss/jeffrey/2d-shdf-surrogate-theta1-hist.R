theta1 = read.table("2d-shdf-surrogate-theta1-sampled.dat")
postscript("2d-shdf-surrogate-theta1-hist.eps",onefile=FALSE,pointsize=14)
hist(theta1$V1, breaks=25, xlim=c(0, 5), freq=FALSE)
dev.off()
