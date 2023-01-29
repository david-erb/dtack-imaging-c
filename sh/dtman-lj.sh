#! /bin/sh
# convert all man files via dvi and ps to lj3 in directory
# yyy/xxx.1 -> yyy/xxx.1.dvi
# then
# yyy/xxx.1.dvi -> yyy/xxx.1.dvi.ps
# then
# yyy/xxx.1.dvi.ps -> yyy/xxx.1.dvi.ps.lj3
# checks dates on xxx.1 versus xxx.1.dvi.ps
# ---------------------------------------------------------------------------
run()
{
  echo "dtman-lj.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

if test $# -ne 2
then
  echo "dtman-lj.sh: usage srcdir outdir"
  exit -1
fi

srcdir=$1
shift
outdir=$1
shift

run "sh $srcdir/sh/dtman-ps.sh $srcdir $outdir"

run "sh $srcdir/sh/dtps-lj3.sh $srcdir $outdir"
