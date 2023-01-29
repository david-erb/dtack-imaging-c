#! /bin/sh
# convert a postscript file to tiff
# full filename of input postscript file is required
# full filename of output tiff file is required
# ---------------------------------------------------------------------------
run()
{
  echo "dtps2tif.sh: $@"
  eval "$@"
}
# ---------------------------------------------------------------------------

dir=$1
shift
input=$1
shift
output=$1
shift
options=$@

cd $dir

run "gs -r300x300 -sDEVICE=tiffg3 -sOutputFile=$output $options - < $input"

