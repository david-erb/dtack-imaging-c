#

say=dtjust10.sh
# --------------------------------------------------------
say()
{
  echo "$say: $@"
}
# --------------------------------------------------------
run()
{
  if [ -n "$DT_SH" ]
  then
    say "$@"
  fi
  eval "$@"
}
# --------------------------------------------------------
if [ $# -lt 7 ]
then
  say "usage: giffile width height left_pad right_pad top_pad bottom_pad pgmfile"
  exit
fi
gif=$1
width=`let $2`
height=`let $3`
left=`let $4`
right=`let $5`
top=`let $6`
bot=`let $7`
pgm=$8
desired_width=`let $2+$4+$5`
desired_height=`let $3+$6+$7`
pad=/tmp/dtjust10.$$.pad.pgm
tmp=/tmp/dtjust10.$$.tmp.pgm
# pgm=/tmp/dtjust10.$$.pgm
# --------------------------------------------------------
# --------------------------------------------------------
# --------------------------------------------------------
# --------------------------------------------------------

run "giftopnm $gif | dttool copy -prec b08 stdin $pgm"
run "dttool copy -w $width $pgm $pgm"

if [ $top -gt 0 ]
then
  set -e
  run "dttool gen -ba 0 -w $desired_width -h $top 1 $pad"
  run "pnmcat -tb $pad $pgm > $tmp"
  run "cp $tmp $pgm"
  run "rm $pad $tmp"
  set +e
fi

if [ $bot -gt 0 ]
then
  set -e
  run "dttool gen -ba 0 -w $desired_width -h $bot 1 $pad"
  run "pnmcat -tb $pgm $pad > $tmp"
  run "cp $tmp $pgm"
  run "rm $pad $tmp"
  set +e
fi

if [ $left -gt 0 ]
then
  set -e
  run "dttool gen -ba 0 -w $left -h $desired_height 1 $pad"
  run "pnmcat -lr $pad $pgm > $tmp"
  run "cp $tmp $pgm"
  run "rm $pad $tmp"
  set +e
fi

if [ $right -gt 0 ]
then
  set -e
  run "dttool gen -ba 0 -w $right -h $desired_height 1 $pad"
  run "pnmcat -lr $pgm $pad > $tmp"
  run "cp $tmp $pgm"
  run "rm $pad $tmp"
  set +e
fi

set -e
run "pnminvert $pgm | dttool regray stdin $pgm"
