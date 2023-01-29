#! /bin/sh
# convert a Postscript file to Laserjet 3 (300 dpi)
# output is put into ps file in same directory
# ---------------------------------------------------------------------------
say()
{
  if test "$DT_SH" = "1"
  then
    echo "dtps2lj4.sh: $@"
  fi
}
# ---------------------------------------------------------------------------
run()
{
  if test "$DT_SH" = "1"
  then
    echo "dtps2lj4.sh: $@"
  fi
  eval "$@"
  if test ! $?
  then
    echo "dtps2lj4.sh: exiting"
    exit 1
  fi
}
# ---------------------------------------------------------------------------

if test $# -ne 2
then
  echo "dtps2lj4.sh: usage psfile ljfile"
  exit 1
fi

psfile=$1
shift
ljfile=$1
shift
options=

run "gs -q -r600x600 -sDEVICE=ljet4 -sOutputFile=$ljfile $options - < $psfile"

