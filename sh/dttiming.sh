#! /bin/sh

# must be run from the dt main directory

if [ $# != 1 ]
then
  echo "usage: dttiming.sh directory_name"
  exit
fi

if [ ! -d $1 ]
then
  echo "dttiming.sh: $1 is not a directory"
  exit
fi

dir=$1
out=$dir/timing.out
bin=$dir/bin
tmp=$dir/timing.tmp
datadir=./data
dtutil=$dir/bin/dtutil

mydtutil()
{
  if [ ! -x $dir/bin/dtutil ]
  then
    echo "dttiming.sh: exiting because could not find $dtutil"
    exit 1
  fi

  $dtutil $@

  if [ $? -ne 0 ] 
  then
    echo "dttiming.sh: exiting because $dtutil failed"
    exit 1
  fi
}

export LD_LIBRARY_PATH
LD_LIBRARY_PATH=$dir/libso:$LD_LIBRARY_PATH

echo ===========================================                      >  $out
echo "date     `date`"                                                >> $out
echo "uname    `uname -a`"                                            >> $out
echo "pwd      `pwd`"                                                 >> $out

run()
{
  mydtutil time $1: being started by dttiming.sh _date_ _time_    
  mydtutil time $1: being started by dttiming.sh _date_ _time_  >> $out
  shift
  $bin/$@ -dialog none -op time -D time+assert -errto $tmp -dbgto $tmp
  if [ -r $tmp ]
  then
    cat $tmp >> $out
    rm -f $tmp
  fi
}

run dttmem   dttmem  mem -reps 1 -n 500
run dttstack dttmem  stack

run dttarith dttarith -n 250 -datadir $datadir
run dttlog   dtthigh1 log -n 250
run dttrun   dtthigh1 run -datadir $datadir

run dtt1d    dtt1d -n 750000
run dttcast  dttcast -n 250 -reps 1
run dttconv  dttconv -datadir $datadir -n 16
run dttadj   dttadj -datadir $datadir
