#! /bin/sh

#..............................................
run()
{
  echo "dtms2prt.sh: $1"
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
tmp=/tmp/dtms2prt

# run refer
run "refer -p $dir/doc/biblio.ref $input > ${tmp}1"

# get rid of the line where refer outputs the references
run "grep -v '\\\*(\[\.' < ${tmp}1 > ${tmp}2"

# run table processor
run "tbl < ${tmp}2 > ${tmp}3"

# make ASCII output with formfeeds, underlines, and overstrikes
run "groff -Tascii -mgs -P-f < ${tmp}3 > ${tmp}4"
 
# add spaces to beginnings and ^M to ends of lines
run "awk '{printf(\"          %s%c\n\", \$0, 13)}' < ${tmp}4 > $output" 

