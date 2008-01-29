theta2 = read.table("2d-shdf-surrogate-theta2-sampled.dat")
postscript("2d-shdf-surrogate-theta2-hist.eps",onefile=FALSE,pointsize=14)
hist(theta2$V1, breaks=25, xlim=c(0, 5), freq=FALSE)
dev.off()
