sigma = read.table("2d-shdf-surrogate-sigma-sampled.dat")
postscript("2d-shdf-surrogate-sigma-hist.eps",onefile=FALSE,pointsize=14)
hist(sigma$V1, breaks=1000, xlim=c(1, 4), freq= FALSE)
dev.off()
