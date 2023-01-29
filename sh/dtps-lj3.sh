#! /bin/sh
# convert all ps files lj3 in directory
# yyy/xxx.ps -> yyy/xxx.ps.lj3
# checks dates on xxx.1 versus xxx.1.ps
# ---------------------------------------------------------------------------
run()
{
  echo "dtps-lj3.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

if test $# -ne 2
then
  echo "dtps-lj3.sh: usage srcdir outdir"
  exit -1
fi

srcdir=$1
shift
outdir=$1
shift

for psfile in $outdir/*.ps
do
  base=`basename $psfile`
  find=`find $outdir -newer $psfile -name $base.lj3`
  if test -z "$find"
  then
    run "sh $srcdir/sh/dtps2lj3.sh $psfile" 
  else
    echo "dtps-lj3.sh: $psfile up to date"
  fi
done
