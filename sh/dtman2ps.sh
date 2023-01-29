#! /bin/sh
# convert a man file via dvi to Postscript
# yyy/xxx.1 -> yyy/xxx.1.dvi
# then
# yyy/xxx.1.dvi -> yyy/xxx.1.dvi.ps
# ---------------------------------------------------------------------------
run()
{
  echo "dtman2ps.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

if test $# -ne 1
then
  echo "usage: dtman2ps.sh manfile"
  exit -1
fi

manfile=$1

run "groff -Tdvi -mandoc < $manfile > $manfile.dvi"
run "dvips -q -O 0in,-0.35in -o $manfile.dvi.ps $manfile.dvi"
