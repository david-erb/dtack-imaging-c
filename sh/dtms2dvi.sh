#! /bin/sh

#..............................................
run()
{
  echo "dtms2dvi.sh: $1"
  eval "$1"
#  if [ $? != 0 ]
#  then
#    exit
#  fi
}
#..............................................

dir=$1
shift
input=$1
shift
output=$1
tmp=/tmp/dtms2dvi

# run refer
run "refer -p $dir/doc/biblio.ref $input > ${tmp}1"

# get rid of the line where refer outputs the references
run "grep -v '\\\*(\[\.' < ${tmp}1 > ${tmp}2"

# run table processor
run "tbl < ${tmp}2 > ${tmp}3"

# make dvi output
run "groff -Tdvi -mgs  < ${tmp}3 > $output"

