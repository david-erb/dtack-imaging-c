#!/bin/sh

# must be run from the dt main directory

if [ $# != 1 ]
then
  echo "usage: dtcheck.sh directory_name"
  exit
fi

dir=$1
make=$dir/make.out
error=$dir/error.out
warning=$dir/warning.out
testfail=$dir/testfail.out
timefail=$dir/timefail.out
test=$dir/test.out
timing=$dir/timing.out
check=$dir/check.out
dtutil=$dir/bin/dtutil

mydtutil()
{
  if [ ! -x $dir/bin/dtutil ]
  then
    echo "dtcheck.sh: exiting because could not find $dtutil"
    exit 1
  fi

  $dtutil "$@"

  if [ $? -ne 0 ] 
  then
    echo "dtcheck.sh: exiting because $dtutil failed"
    exit 1
  fi
}

rm -f $error $warning $testfail $timefail $check

if [ -r $make ]
then
  echo "dtcheck.sh: scanning $make ..................." 
  echo "dtcheck.sh: scanning $make ..................." >> $check
  mydtutil grep error   $make | grep -v Werror   >> $error
  mydtutil grep failed  $make                    >> $error
  mydtutil grep warning $make                    >> $warning
  cat $error $warning                            >> $check
fi   

if [ -r $test ]
then
  echo "dtcheck.sh: scanning $test ..................." 
  echo "dtcheck.sh: scanning $test ..................." >> $check
  mydtutil grep "error:"     $test               >> $testfail
  mydtutil grep "warning:"   $test               >> $testfail
  mydtutil grep ": *"        $test               >> $testfail
  mydtutil endcheck          $test               >> $testfail
  cat $testfail                                  >> $check
fi

if [ -r $timing ]
then
  echo "dtcheck.sh: scanning $timing ................."
  echo "dtcheck.sh: scanning $timing ................." >> $check
  mydtutil grep "error:"     $timing             >> $timefail
  mydtutil grep ": *"        $timing             >> $timefail
  mydtutil endcheck          $timing             >> $timefail
  cat $timefail                                  >> $check
fi

cat $check






