#! /bin/sh

# must be run from the dt main directory

if [ $# != 1 ]
then
  echo "usage: dttest.sh directory_name"
  exit
fi

if [ ! -d $1 ]
then
  echo "dttest.sh: $1 is not a directory"
  exit
fi

dir=$1
out=$dir/test.out
bin=$dir/bin
sh=$dir/sh
tmp=$dir/test.tmp
datadir=./data
tmpdir=$dir/tmp
dtutil=$dir/bin/dtutil

if [ -n "$SRC" ]			# SRC is set?
then
  datadir=$SRC/data			# get gold standard data here
fi

mydtutil()
{
  if [ ! -x $dir/bin/dtutil ]
  then
    echo "dttest.sh: exiting because could not find $dtutil"
    exit 1
  fi

  $dtutil $@

  if [ $? -ne 0 ] 
  then
    echo "dttest.sh: exiting because $dtutil failed"
    exit 1
  fi
}

export LD_LIBRARY_PATH
LD_LIBRARY_PATH=$dir/libso:$LD_LIBRARY_PATH

echo ===========================================                      >  $out
echo "date     `date`"                                                >> $out
echo "uname    `uname -a`"                                            >> $out
echo "pwd      `pwd`"                                                 >> $out

# ...........................................................................
run()
{
  mydtutil time $1: being started by dttest.sh _date_ _time_    
  mydtutil time $1: being started by dttest.sh _date_ _time_  >> $out
  shift
  $bin/$@ -dialog none -D assert -errto $tmp -dbgto $tmp $arg
  if [ -r $tmp ]
  then
    cat $tmp >> $out
    rm -f $tmp
  fi
}
# ...........................................................................
cmd()
{
  mydtutil time $1.sh: being started by dttest.sh _date_ _time_    
  mydtutil time $1.sh: being started by dttest.sh _date_ _time_  >> $out
  export DT_DIALOG=none
  export DT_D=assert
  export DT_DBGTO=stdout
  export DT_ERRTO=stdout
  export DT_SAYTO=stdout
  tmp=$dir/tmp/$1.out
  sh $sh/$1.sh $dir > $tmp
  if [ -r $tmp ]
  then
    cat $tmp >> $out
    mydtutil cmp $tmp $data/$1.out >> $out
  else
    echo "dttest.sh: error: could not find $tmp" >> $out
  fi
}
# ...........................................................................

run dtt1d       dtt1d  

run dttstr      dttlow str   
run dttheap     dttlow heap   
run dttparam    dttlow param 
run dttlist     dttlow list  
run dttos       dttlow os      -output $tmpdir
run dttipc      dttlow ipc

# run dttnote     dttinter note  
# arg=text,curses
# run dttdia      dttinter dia                   
# run dttalpha    dttinter alpha -output $tmpdir 
# arg=

run dttmem      dttmem mem     -output $tmpdir
run dttstack    dttmem stack   -output $tmpdir

run dttstat     dtthigh2 stat
run dttcooc     dtthigh2 cooc
run dttjoint    dtthigh2 joint
run dttresample dtthigh2 resample
run dttmatrix   dtthigh2 matrix

run dtthist     dtthigh1 hist
run dttplot     dtthigh1 plot 
run dttlog      dtthigh1 log  
run dttpgm      dtthigh1 pgm   -datadir $datadir
run dttrun      dtthigh1 run   -datadir $datadir

run dttadj      dttadj         -datadir $datadir
run dttarith    dttarith       -datadir $datadir
run dttcast     dttcast 
run dttcmp      dttcmp
run dttconv     dttconv       -datadir $datadir
run dttmorph    dttmorph

run dttxchg     dttxchg       -datadir $datadir -output $tmpdir

# run dttwin      dttwin  -screen dummy
# run dttwin      dttwin  -screen canvas,dummy

run dttview     dttview -screen dummy  -output $tmpdir
# run dttview     dttview -screen canvas,dummy -output $tmpdir

exit 0
