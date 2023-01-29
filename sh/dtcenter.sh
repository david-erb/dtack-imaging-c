#

say=dtcenter.sh
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
if [ $# -lt 5 ]
then
  say "usage: input.pgm width height background.pgm output.gif"
  exit
fi
pgm=$1
width=`let $2`
height=`let $3`
background=$4
gif=$5
# --------------------------------------------------------
# --------------------------------------------------------
# --------------------------------------------------------
# --------------------------------------------------------

run "dttool paste -op center $pgm $background stdout | ppmtogif -map 4color.ppm > $gif 2> /dev/null"

