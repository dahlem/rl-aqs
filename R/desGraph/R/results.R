## Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
## Keywords: file i/o
## Created: 30.04.2008


eval.dir <- function(filename)
{
  # create the evaluation directory for the current R session
  if (!exists("evalDir")) {
    evalDir <<- paste("./eval/", format(Sys.time(), "%Y%m%d_%H%M%S/"), sep="")
    dir.create(evalDir, recursive=TRUE, showWarnings=FALSE)
  }

  return(paste(evalDir, filename, sep=""))
}
