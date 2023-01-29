#! /bin/sh
# convert a texinfo dvi file into postscript
# ---------------------------------------------------------------------------
run()
{
  echo "dtdvt2ps.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

run dvips -d 4 -o $1.pst -D 300 $1.dvt

