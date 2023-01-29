#! /bin/sh
old=`pwd`

if [ $# -lt 6 ]
then
  echo "usage: figs.sh fig dir src out doc density"
  exit
fi
target=$1
shift
#..............................................
if [ $# -lt 1 ]
then
  echo "usage: figs.sh directory_name"
  exit
fi
dir=$1
shift

#..............................................
if [ $# -lt 1 ]
then
  src=.
else
  src=$1
fi
if [ ! -d $src/$dir ]
then
  echo "figs.sh: $src/$dir is not a directory"
  exit
fi
cd $src
src=`pwd`
cd $old
shift
#..............................................
if [ $# -lt 1 ]
then
  out=.
else
  out=$1
fi
if [ ! -d $out/bin ]
then
  echo "figs.sh: $out/bin is not a directory"
  exit
fi
cd $out
out=`pwd`
cd $old
shift
#..............................................
if [ $# -lt 1 ]
then
  doc=.
else
  doc=$1
fi
if [ ! -d $doc/$dir ]
then
  echo "figs.sh: $doc/$dir is not a directory"
  exit
fi
cd $doc
doc=`pwd`
cd $old
shift
#..............................................
if [ "$1" != "300" -a  "$1" != "600" ]
then
  echo "figs.sh: $1 is not a valid density"
  exit
fi
if [ "$1" = "300" ]
then
  density=3
  scale=0.250
  mag3=0.33
  mag2=0.5
  mag1=1.0
fi
if [ "$1" = "600" ]
then
  mag3=0.165
  mag2=0.25
  mag1=0.5
  density=6
  scale=0.125
fi
shift
#..............................................

#..............................................
run()
{
  echo "figs.sh: $1"
  eval "$1"
  if [ $? != 0 ]
  then
    exit
  fi
}
#..............................................
image()
{
  run "$dttool gen -ba 192 -w 128 -h 128 1 /tmp/tmp1"
  run "$dttool text -fo 0 -ba 255 -y 120 -x 40 /tmp/tmp1 \"$3\" /tmp/tmp2"
  run "$dttool resample -op bilinear /tmp/tmp2 0.25 0.25 /tmp/tmp3"
  run "pgmtopbm -c4 < /tmp/tmp3 > /tmp/tmp4"
  run "pnmtops -scale $scale -noturn /tmp/tmp4 > $doc/$dir/$3"
}
#..............................................
gray1_ps()
{
  image 256 256 "gray1.ps"
}
#..............................................
seg1a_ps()
{
  image 256 256 "seg1a.ps"
}
#..............................................
seg1b_ps()
{
  image 256 256 "seg1b.ps"
}
#..............................................
_dither_gray()
{
# run "$out/bin/dtesc94 -foreground 0 -background 255 $1 $2"
  run "$out/bin/dtesc94 $1 $2"
}
_dither_binary()
{
# run "$dttool lookup $1 invert stdout | pgmtopbm -c3 > $2"
  run "pgmtopbm -c3 < $1 > $2"
}
_reformat_pgm()
{
  pgm=$pgmdir/`echo $2|sed 's/[.]ps$/.pgm.gz/'`
  run "dttool regray -precision b08 $1 stdout | gzip -c > $pgm"
}
_pgmtops()
{
  run "pnmtops -scale $scale -noturn < $1 | gzip -c > ${2}${density}.gz"
  run "zcat <${2}${density}.gz | grep Bounding | cut -f2-5 -d' ' > ${2}.box"
}
#..............................................
gray2_ps()
{
  gray=$src/doc/data/dominoes/white.bpe
  run "_reformat_pgm $gray $1"
  run "$dttool resample -op bilinear $gray $mag1 $mag1 $tmp1"
  run "$dttool regray $tmp1 $tmp2"
  _dither_gray $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
seg2a_ps()
{
  gray=$src/doc/data/dominoes/white.bpe
  run "$out/bin/dtsez90 -n 15 -dx 11 $gray $tmp1 $tmp2"
  run "$out/bin/dtlev85 -min 100 -op label $gray $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample -op bilinear $tmp1 $mag1 $mag1 $tmp2"
  _dither_binary $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
seg2b_ps()
{
  gray=$src/doc/data/dominoes/white.bpe
  run "$out/bin/dtsez90 -n 5 -dx 3 $gray $tmp1 $tmp2"
  run "$out/bin/dtlev85 -background 0 -op total $gray $tmp1"
  run "$out/bin/dtlev85 -min 100 -op label $gray $tmp1"
  run "$out/bin/dtlev85 -min 200 -max 300 -op blob $gray $tmp1"
  run "$out/bin/dtlev85 -min 200 -max 300 -op label $gray $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample -op bilinear $tmp1 $mag1 $mag1 $tmp2"
  _dither_binary $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
gray3_ps()
{
  gray=$src/doc/data/letters/white.bpe
  run "_reformat_pgm $gray $1"
  run "$dttool resample -op bilinear $gray $mag1 $mag1 $tmp1"
  run "$dttool regray $tmp1 $tmp2"
  _dither_gray $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
seg3a_ps()
{
  gray=$src/doc/data/letters/white.bpe
  run "$dttool thresh $gray 150 $tmp1"
  run "$out/bin/dtlev85 $gray $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample -op bilinear $tmp1 $mag1 $mag1 $tmp2"
  _dither_binary $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
seg3b_ps()
{
  gray=$src/doc/data/letters/white.bpe
  run "$dttool thresh $gray 180 $tmp1"
  run "$out/bin/dtlev85 $gray $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample -op bilinear $tmp1 $mag1 $mag1 $tmp2"
  _dither_binary $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
gray4a_ps()
{
  run "$dttool gen -omega 4 -phi 1.57 -max 100 -w 512 -h 512 ysine $tmp1"
  run "$dttool box -color 0 $tmp1 220 0 221 511 $tmp1"
  run "$dttool box -color 0 $tmp1 222 0 223 511 $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample -op bilinear $tmp1 $mag1 $mag1 $tmp2"
  _dither_gray $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
seg4a_ps()
{
  run "$dttool gen -omega 4 -phi 1.57 -max 100 -w 512 -h 512 ysine $tmp1"
  gray=$tmp1
  run "$dttool gen -dx 220 -w 512 -h 512 -background 80 -foreground 224 2v $tmp2"
  run "$out/bin/dtlev85 -op label $gray $tmp2"
  run "$out/bin/dtlev85 $gray $tmp2"
  run "$dttool gen -dx 110 -w 256 -h 256 -background 80 -foreground 224 2v $tmp2"
  run "$dttool gen -w 40 -h 16 -background 32 1 $tmp3"
  run "$dttool text -foreground 255 $tmp3 area1 $tmp3"
  run "$dttool paste -x 35 -y 124 $tmp3 $tmp2 $tmp2"
  run "$dttool gen -w 40 -h 16 -background 224 1 $tmp3"
  run "$dttool text -foreground 0 $tmp3 area2 $tmp3"
  run "$dttool paste -x 163 -y 124 $tmp3 $tmp2 $tmp2"
  run "_reformat_pgm $tmp2 $1"
  run "$dttool resample $tmp2 $mag2 $mag2 $tmp3"
  _dither_binary $tmp3 $tmp4
  _pgmtops $tmp4 $doc/$dir/$1
}
#..............................................
gray4b_ps()
{
  run "$dttool gen -omega 4 -phi 1.57 -max 100 -w 512 -h 512 ysine $tmp1"
  run "$dttool box -color 0 $tmp1 200 0 201 511 $tmp1"
  run "$dttool box -color 0 $tmp1 202 0 203 511 $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample -op bilinear $tmp1 $mag1 $mag1 $tmp2"
  _dither_gray $tmp2 $tmp3
  _pgmtops $tmp3 $doc/$dir/$1
}
#..............................................
seg4b_ps()
{
  run "$dttool gen -omega 4 -phi 1.57 -max 100 -w 512 -h 512 ysine $tmp1"
  gray=$tmp1
  run "$dttool gen -dx 200 -w 512 -h 512 -background 80 -foreground 224 2v $tmp2"
  run "$out/bin/dtlev85 -op label $gray $tmp2"
  run "$out/bin/dtlev85 $gray $tmp2"
  run "$dttool gen -dx 100 -w 256 -h 256 -background 80 -foreground 224 2v $tmp2"
  run "$dttool gen -w 40 -h 16 -background 32 1 $tmp3"
  run "$dttool text -foreground 255 $tmp3 area1 $tmp3"
  run "$dttool paste -x 30 -y 124 $tmp3 $tmp2 $tmp2"
  run "$dttool gen -w 40 -h 16 -background 224 1 $tmp3"
  run "$dttool text -foreground 0 $tmp3 area2 $tmp3"
  run "$dttool paste -x 158 -y 124 $tmp3 $tmp2 $tmp2"
  run "_reformat_pgm $tmp2 $1"
  run "$dttool resample $tmp2 $mag2 $mag2 $tmp3"
  _dither_binary $tmp3 $tmp4
  _pgmtops $tmp4 $doc/$dir/$1
}
#..............................................
# billiard balls
gray5_ps()
{
  gray=$src/doc/data/billiard/g.bpe
  run "$dttool convolve -prec s2 -w1 -h5 $gray ave $tmp1"
  run "$dttool convolve -prec s2 -w5 -h5 $tmp1 $src/doc/data/billiard/highboost5.krn $tmp2"
  run "_reformat_pgm $tmp2 $1"
  run "$dttool resample -op bilinear $tmp2 $mag3 $mag3 $tmp3"
  run "$dttool regray $tmp3 $tmp4"
  _dither_gray $tmp4 $tmp5
  _pgmtops $tmp5 $doc/$dir/$1
}
copy()
{ 
  dest=`echo $2|sed 's/[.]ps$//'`
  run "cp $doc/$dir/$1.ps${density}.gz $doc/$dir/$dest.ps${density}.gz"
  run "cp $doc/$dir/$1.ps.box          $doc/$dir/$dest.ps.box"
  run "cp $pgmdir/$1.pgm.gz            $pgmdir/$dest.pgm.gz"
}
gray5x2_ps()
{
  copy gray5 $1
}
gray5x3_ps()
{
  copy gray5 $1
}
#..............................................
seg5a_ps()
{
  gray=$src/doc/data/billiard/g.bpe
  seg=$src/doc/data/billiard/seg1.bpe
  run "$dttool lookup $seg invert $tmp1"
  run "$dttool linear $tmp1 .8 0 $tmp2"
  run "_reformat_pgm $tmp2 $1"
  run "$dttool resample $tmp2 $mag3 $mag3 $tmp3"
  run "$out/bin/dtlev85 -op label $gray $seg"
  _dither_binary $tmp3 $tmp4
  _pgmtops $tmp4 $doc/$dir/seg5a.ps
}
seg5ax2_ps()
{
  copy seg5a $1
}
#..............................................
seg5b_ps()
{
  gray=$src/doc/data/billiard/g.bpe
  seg=$src/doc/data/billiard/seg2.bpe
  run "$dttool lookup $seg invert $tmp1"
  run "$dttool linear $tmp1 .8 0 $tmp2"
  run "_reformat_pgm $tmp2 $1"
  run "$dttool resample $tmp2 $mag3 $mag3 $tmp3"
  run "$out/bin/dtlev85 -op label $gray $seg"
  _dither_binary $tmp3 $tmp4
  _pgmtops $tmp4 $doc/$dir/seg5b.ps
}
seg5bx2_ps()
{
  copy seg5b $1
}
#..............................................
seg5c_ps()
{
  gray=$src/doc/data/billiard/g.bpe
  seg=$src/doc/data/billiard/seg1.bpe
  run "$dttool enclose -op fill $seg $seg $tmp1"
  run "$dttool linear $tmp1 24 32 $tmp2"
  run "$dttool lookup $tmp2 invert $tmp3"
  run "$dttool enclose -op number -color 0 $seg $tmp3 $tmp4"
  run "$dttool text -x80 -y100 -foreground 0 $tmp4 4 $tmp5"
  run "_reformat_pgm $tmp5 $1"
  run "$dttool resample $tmp5 $mag3 $mag3 $tmp6"
  run "$out/bin/dtlev85 -op label $gray $tmp1"
  run "$out/bin/dtlev85 -op total -ba 1 $gray $tmp1"
  _dither_binary $tmp6 $tmp7
  _pgmtops $tmp7 $doc/$dir/$1
}
#..............................................
seg5d_ps()
{
  gray=$src/doc/data/billiard/g.bpe
  seg=$src/doc/data/billiard/seg2.bpe
  run "$dttool enclose -op fill $seg $seg $tmp1"
  run "$dttool linear $tmp1 32 32 $tmp2"
  run "$dttool lookup $tmp2 invert $tmp3"
  run "$dttool enclose -op number -color 0 $seg $tmp3 $tmp4"
  run "_reformat_pgm $tmp4 $1"
  run "$dttool resample $tmp4 $mag3 $mag3 $tmp5"
  run "$out/bin/dtlev85 -op label $gray $tmp1"
  _dither_binary $tmp5 $tmp6
  _pgmtops $tmp6 $doc/$dir/$1
}
#..............................................
draw6_ps()
{
  image=$tmp1
  run "$dttool gen -w512 -h512 -background 200 1 $tmp1"
  run "$dttool gen -w88 -h88 -ba 64 1 $tmp2"
  run "$dttool gen -w55 -h55 -ba 255 1 $tmp3"
  run "$dttool gen -w88 -h88 -ba 255 -sigma 40 6 $tmp4"
  run "$dttool thresh -ba 200 $tmp4 77 $tmp4"
  run "$dttool gen -w44 -h44 -ba 255 -sigma 20 6 $tmp5"
  run "$dttool thresh -ba 200 -fo 64 $tmp5 77 $tmp5"
  run "$dttool paste -x 100 -y 100 $tmp2 $tmp1 $tmp1"
  run "$dttool paste -x 300 -y 50 $tmp3 $tmp1 $tmp1"
  run "$dttool paste -x 90 -y 290 $tmp3 $tmp1 $tmp1"
  run "$dttool paste -x 270 -y 340 $tmp4 $tmp1 $tmp1"
  run "$dttool paste -x 400 -y 200 $tmp5 $tmp1 $tmp1"
  run "_reformat_pgm $tmp1 $1"
  run "$dttool resample $tmp1 $mag1 $mag1 $tmp6"
  _dither_binary $tmp6 $tmp7
  _pgmtops $tmp7 $doc/$dir/$1
}
#..............................................
set -e
export DT_DIALOG=none
export LD_LIBRARY_PATH=$out/libso:$LD_LIBRARY_PATH
dttool=$out/bin/dttool
pgmdir=$doc/doc/pgm/dtlev85
cmd=`echo $target | sed 's/\./_/'`

if [ ! -d $pgmdir ]
then
  mkdir $pgmdir
fi

tmp1=$doc/$dir/tmp1.pgm
tmp2=$doc/$dir/tmp2.pgm
tmp3=$doc/$dir/tmp3.pgm
tmp4=$doc/$dir/tmp4.pgm
tmp5=$doc/$dir/tmp5.pgm
tmp6=$doc/$dir/tmp6.pgm
tmp7=$doc/$dir/tmp7.pgm

echo "figs.sh: ${cmd} $target"
eval "$cmd $target"

rm -f $doc/$dir/tmp*

#..............................................

