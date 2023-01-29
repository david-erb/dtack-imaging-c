#! /bin/sh

# make a .txt file from a .alp and its .ref
# example:
# dx -falpha -orelax relax.doc
# sh ../../sh/dttxt.sh ../.. relax relax.txt

#..............................................
run()
{
  echo "dttxt.sh: $1"
  eval "$1"
}
#..............................................

set -e

dir=$1
shift
input=$1
shift
output=$1
tmp=/tmp/dttxt

alp=$input.alp
ref=$input.ref

# run refer
run "refer -p $dir/doc/biblio.ref $ref > ${tmp}1"

# get rid of the line where refer outputs the references
run "grep -v '\\\*(\[\.' < ${tmp}1 > ${tmp}2"

# make ASCII output without formfeeds, underlines, or overstrikes
run "groff -Tascii -mgs -P-b -P-u < ${tmp}2 > ${tmp}3"
 
# remove blank lines and References line
run "awk 'NF {printf(\"%s\n\", \$0)}' < ${tmp}3 | grep -v References > ${tmp}4"

# add alpha 
run "cat $alp ${tmp}4 > $output" 

