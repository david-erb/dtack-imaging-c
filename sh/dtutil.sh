#! /bin/sh

# must be run from the dt main directory

if [ $# -lt 1 ]
then
  echo "usage: dtutil.sh directory_name [dtutil-args]"
  exit
fi

if [ ! -d $1 ]
then
  echo "dtutil.sh: $1 is not a directory"
  exit
fi

dir=$1
dtutil="$dir/dtutil"
shift

make SRC=`pwd` OUT=$dir ACT=$dtutil

if [ $? != 0 ]
then
  echo "dtutil.sh: failed to make $dtutil"
  exit 1
else
  $dtutil $@
  if [ $? != 0 ]
  then
    echo "dtutil.sh: unable to execute $dtutil"
    exit 1
  fi
fi

