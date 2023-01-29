set -x
output=dttestlj.lj3
rm -f $output
dttool gen -fo 0 -ba 255 -dx 2 -dy 2 -w 512 -h 512 -x 800 -y 100 \
   -precision b01 check lj3,,stdout >> $output
dttool gen -fo 0 -ba 255 -dx 4 -dy 4 -w 512 -h 512 -x 800 -y 700 \
   -precision b01 check lj3,,stdout >> $output
dttool resample -op bilinear /img/std/mit.pgm .5 .5 stdout | 
   dtesc94 -ba 255 -fo 0 stdin stdout | 
   dttool copy -precision b01 -w 504 -x 800 -y 1300  stdin lj3,,stdout >> $output
dtesc94 -ba 255 -fo 0 /img/dice/white.pgm stdout | 
   dttool copy -precision b01 -x 800 -y 1900  stdin lj3,,stdout >> $output

output=dttestlj.lj6
rm -f $output
dttool gen -fo 0 -ba 255 -dx 2 -dy 2 -w 1024 -h 1024 -x 1600 -y 200 \
   -precision b01 check lj6,,stdout >> $output
dttool gen -fo 0 -ba 255 -dx 4 -dy 4 -w 1024 -h 1024 -x 1600 -y 1400 \
   -precision b01 check lj6,,stdout >> $output
dttool resample -op bilinear /img/std/mit.pgm .25 .25 stdout | 
   dtesc94 -ba 255 -fo 0 stdin stdout | 
   dttool copy -precision b01 -w 1016 -x 1600 -y 2600  stdin lj6,,stdout >> $output
dttool resample -op bilinear /img/dice/white.pgm .5 .5 stdout | 
   dtesc94 -ba 255 -fo 0 stdin stdout | 
   dttool copy -precision b01 -w 1016 -x 1600 -y 3800  stdin lj6,,stdout >> $output

