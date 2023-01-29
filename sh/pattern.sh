set -x

w=16
h=16
let h1=$h-1

image=test1
dttool gen -ba 1 -w $w -h $h 1 $image.pgm
let col=1*$w/4
dttool box -color  64 $image.pgm $col 0 $col $h1 $image.pgm
let col=2*$w/4
dttool box -color 128 $image.pgm $col 0 $col $h1 $image.pgm
let col=3*$w/4
dttool box -color 192 $image.pgm $col 0 $col $h1 $image.pgm
ppmtogif <$image.pgm >$image.gif

image=test2
dttool gen -ba 1 -w $w -h $h 1 $image.pgm
let col=1*$w/4
dttool box -color  64 $image.pgm $col 0 $col $h1 $image.pgm
let col=$col+1
dttool box -color 128 $image.pgm $col 0 $col $h1 $image.pgm
let col=$col+1
dttool box -color 192 $image.pgm $col 0 $col $h1 $image.pgm
ppmtogif <$image.pgm >$image.gif

image=test2
dttool gen -ba 1 -w $w -h $h 1 $image.pgm
let col=1*$w/4
dttool box -color  64 $image.pgm $col 0 $col $h1 $image.pgm
let col=$col+1
dttool box -color 128 $image.pgm $col 0 $col $h1 $image.pgm
let col=$col+1
dttool box -color 192 $image.pgm $col 0 $col $h1 $image.pgm
ppmtogif <$image.pgm >$image.gif
