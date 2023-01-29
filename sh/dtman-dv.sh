#! /bin/sh
# convert all man files to dvi in directory
# checks dates on xxx.1 versus xxx.1.dvi
# ---------------------------------------------------------------------------
run()
{
  echo "dtman-dv.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

if test $# -ne 2
then
  echo "usage: dtman-dv.sh srcdir outdir"
  exit 1
fi

srcdir=$1
shift
outdir=$1
shift

for manfile in $outdir/*.[13]
do
  if test -z `find $outdir -newer $manfile -name $manfile.dvi`
  then
    run "sh $srcdir/sh/dtman2dv.sh $manfile" 
  fi
done
