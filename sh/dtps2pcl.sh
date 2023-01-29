#! /bin/sh
# convert a postscript file to laserjet3
# full filename of input postscript file is required
# full filename of output laserjet file is required
# ---------------------------------------------------------------------------
run()
{
  echo "dtps2pcl.sh: $@"
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

run "gs -r600x600 -sDEVICE=ljet4 -sOutputFile=$output $options - < $input"

