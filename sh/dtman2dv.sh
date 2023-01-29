#! /bin/sh
# convert a man file to dvi
# yyy/xxx.1 -> yyy/xxx.1.dvi
# ---------------------------------------------------------------------------
run()
{
  echo "dtman2dv.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

manfile=$1

run "groff -Tdvi -mandoc < $manfile > $manfile.dvi"
