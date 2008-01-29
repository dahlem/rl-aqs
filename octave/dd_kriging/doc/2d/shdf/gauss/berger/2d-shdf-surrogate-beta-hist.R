beta = read.table("2d-shdf-surrogate-beta-sampled.dat")
postscript("2d-shdf-surrogate-beta-hist.eps",onefile=FALSE,pointsize=14)
hist(beta$V1, breaks=100, xlim=c(1, 2.5), freq = FALSE)
dev.off()
