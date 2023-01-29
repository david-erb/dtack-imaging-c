#! /bin/sh

# must be run from the dt main directory

if [ $# != 1 ]
then
  echo "usage: dtcmd.sh directory_name"
  exit
fi

if [ ! -d $1 ]
then
  echo "dtcmd.sh: $1 is not a directory"
  exit
fi

dir=$1
exe=
bin=$dir/bin
libso=$dir/libso
data=data

# ...........................................................................
cmd()
{
  out=$dir/tmp/$1.out
  export DT_DIALOG=none
  export DT_D=assert
  export DT_DBGTO=stdout
  export DT_ERRTO=stdout
  export DT_SAYTO=stdout
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libso
  sh $data/$1.cmd $dir $dir/tmp $exe > $out
  $bin/dtutil cmp $out data/$1.out 
}
# ...........................................................................

cmd dttool0				# dttool helps
cmd dttool1				# dttool non-window

