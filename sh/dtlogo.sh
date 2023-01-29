set -x -e
dir=/usr/erb/prj/dt
cd $dir/doc/data/logo
/usr/erb/prj/ico2pgm/ico2pgm $dir/config/dtack.ico > logo32.pgm
dttool copy -dy 14 logo32.pgm gravel32.pgm
dttool resample -op bilinear gravel32.pgm .4 .4 gravel80.pgm
dttool resample -op bilinear logo32.pgm .4 .4 logo80.pgm
dttool resample -op bilinear logo32.pgm .2 .2 logo160.pgm

dtesc94 -lambda 10 gravel80.pgm gravel80b.pgm
pnmtops -scale 0.25 -noturn < gravel80b.pgm > gravel80.ps

dtesc94 -lambda 10 logo80.pgm logo80b.pgm
pnmtops -scale 0.25 -noturn < logo80b.pgm > logo80.ps

dtesc94 -lambda 10 logo160.pgm logo160b.pgm
pnmtops -scale 0.25 -noturn < logo160b.pgm > logo160.ps

