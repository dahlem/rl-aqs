scriptRepHome=$(pwd)

for d in `find . -maxdepth 1 -type d -regex "./[0-9]*"`; do
    cd $d;
    echo `pwd`;
    R CMD BATCH ../../../../R/plot-single-des.R;
    cd $scriptRepHome
done
