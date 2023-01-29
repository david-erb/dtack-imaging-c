set -e 
# set -x
xchg=:
export DT_DIALOG=none
# ---------------------------------------------------------------------------
run()
{
  echo "dt_lev85.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------
checkerboard()
{
  dttool gen -dx 32 -dy 32 -w 128 -h 128 checker :cb1
  dttool gen -dx 16 -dy 16 -w 128 -h 128 checker :cb2
  dttool gen -w 64 -h 128 -background 128 1 :1
  dttool paste -x 0 -y 0 :1 :cb1 :cb1b
  dttool paste -x 0 -y 0 :1 :cb2 :cb2b
}
# ---------------------------------------------------------------------------
segment()
{
  run dt_sez90 -n $3 -dx $4 $1 $2
  cmd="dt_lev85 -op env -background 1 -min 3 $1 $2"
  echo "dt_lev85.sh: $cmd"
  eval `$cmd`
  run dttool text -y 240 $2 \"n=$3 dx=$4 u2=$dt_lev85_u2 n2=$dt_lev85_n2\" $2
}
segmented()
{
  run dttool copy ./data/dice256.bpe ${xchg}dice256.bpe
  segment ${xchg}dice256.bpe ${xchg}seg1 31 7
  segment ${xchg}dice256.bpe ${xchg}seg2 15 3
}
# ---------------------------------------------------------------------------
if [ $# -lt 1 ]
then
  echo "dt_lev85.sh: argument count wrong"
  exit 1
fi

d=.d

op=$1
shift
case $op in
  checkerboard | \
  ch*)
    checkerboard $@
  ;;
  segmented | \
  se*)
    segmented $@
  ;;
  *)
    echo "dt_lev85.sh: unknown operation \"$op\""
    exit 1
  ;;
esac