#! /bin/sh
# convert all man files via dvi to ps in directory
# yyy/xxx.1 -> yyy/xxx.1.dvi
# then
# yyy/xxx.1.dvi -> yyy/xxx.1.dvi.ps
# checks dates on xxx.1 versus xxx.1.ps
# cannot use general dvi2ps because man pages must be shifted up by dvips
# ---------------------------------------------------------------------------
run()
{
  echo "dtman3ps.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

if test $# -ne 2
then
  echo "dtman3ps.sh: usage srcdir outdir"
  exit 1
fi

srcdir=$1
shift
outdir=$1
shift

for manfile in $outdir/*.[13]
do
  base=`basename $manfile`
  find=`find $outdir -newer $manfile -name $base.dvi.ps`
  if test -z "$find"
  then
    run "sh $srcdir/sh/dtman2ps.sh $manfile" 
  else
    echo "dtman3ps.sh: $manfile up to date"
  fi
done
