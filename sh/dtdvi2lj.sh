#! /bin/sh
# convert any dvi file into laserjet3
# ---------------------------------------------------------------------------
run()
{
  echo "dtdvi2lj.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

file=$1
shift

if test `uname` = QNX
then
  tex=/usr/local/lib/tex
  res=300
  export TEXFONTS=$tex/fonts/cmfonts/tfm:$tex/fonts/utilityfonts/tfm
  export TEXPKS=$tex/fonts/cmfonts/pk$res:$tex/fonts/utilityfonts/pk$res
  run "dvips -M -D $res $@ -o $file.ps $file.dvi"
  run "gslj4 -r${res}x${res} -q -sDEVICE=ljet4 -sOutputFile=$file.lj - < $file.ps"
else
  run dvilj $file.dvi
fi


