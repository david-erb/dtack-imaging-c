#! /bin/sh
# convert a latex dvi file into postscript
# latex dvi files normally contain references to external EPSF files
# postscript files are completely self-contained

# MakeTeXPK %n %d %b %m dtljfive

# ---------------------------------------------------------------------------
run()
{
  echo "dtdvl2ps.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------
dir=$1
shift
input=$1
shift
output=$1
shift
resolution=${1:-1200}
shift
options=$@

cd $dir

run dvips -d 4 -o $output $options -D $resolution $input

