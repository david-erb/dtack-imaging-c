#! /bin/sh

#..............................................
run()
{
  echo "dtrefer.sh: $1"
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

base=`echo $input | sed 's/\.[^.].*//'`

set -e
rm -f $base.aux $base.ind $base.idx $base.toc $base.lof $base.lot
touch $base.ind
touch $base.toc
touch style.ltx
release="`cat $dir/release` `date +%y%m%d`"
cp $dir/doc/data/logo/logo160.ps logo1.ps
cp $dir/doc/data/logo/logo80.ps logo2.ps

echo "dxrefer.sh: refer pass 1 ============================================="
run "refer \\\def\\\dtrelease{$release}\\\input $input"
echo "dxrefer.sh: bibtex ==================================================="
set +e
export BIBINPUTS=$dir/doc
run "bibtex -verbose $base"
set -e

echo "dxrefer.sh: refer pass 2 ============================================="
run "refer \\\def\\\dtrelease{$release}\\\input $input"

if test -r $base.idx
then
  echo "dxrefer.sh: makeindex ================================================="
  export INDEXSTYLE=$dir:/usr/local/lib/texmf/makeindx
  run "makeindex -s doc/data/dtack.mst $base"
fi

for i in $@
do
  output=`echo $i|sed s/:\.\*//`
  style=`echo $i|sed s/.\*\://`
  if [ -n "$style" ]
  then
    cp $dir/doc/data/$style style.ltx
  else
    rm style.ltx
    touch style.ltx
  fi
  echo "dxrefer.sh: refer pass 3 ============================================="
  run "refer \\\def\\\dtrelease{$release}\\\nofiles\\\input $input"
  mv $base.dvi $output
done

