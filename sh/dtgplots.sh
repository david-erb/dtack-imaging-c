usage()
{
  echo "usage: dtgplots.sh dtview|ps|aplot"
  exit 1
}

if test $# -ne 1
then
  echo "dtgplots.sh: argument count wrong"
  usage
fi

dest=$1

case $dest in
  dtview) ;;
  ps) ;;
  aplot) ;;
  *)
    echo "dtgplots.sh: invalid plot type \"$dest\""
    usage
  ;;
esac

# ----------------------------------------------------------------------------
x()
{
  :
}

# ----------------------------------------------------------------------------
standard()
{
  desc=$1
  shift
  sh sh/dtgplot1.sh $dest:${desc}_cast  cast  $@
  sh sh/dtgplot1.sh $dest:${desc}_conv  conv  $@
  sh sh/dtgplot1.sh $dest:${desc}_mem   mem   $@
  sh sh/dtgplot1.sh $dest:${desc}_stack stack $@
  sh sh/dtgplot1.sh $dest:${desc}_log   log   $@
  sh sh/dtgplot1.sh $dest:${desc}_1d    1d    $@
  sh sh/dtgplot1.sh $dest:${desc}_adj   adj   $@
}
# ----------------------------------------------------------------------------
# takes a raw timing.out
adj_method()
{
  (
  cat < $@ | grep dttadj | grep runs | sed s/runs// | sed s/compiler/runs/
  cat < $@ | grep dttadj | grep runf | sed s/runf// | sed s/compiler/runf/
  cat < $@ | grep dttadj | grep fill | sed s/fill// | sed s/compiler/fill/
  ) > $tmp1
  sh sh/dtgplot1.sh $dest:adj_method adj $tmp1
}
# ----------------------------------------------------------------------------
# takes a raw timing.out
adj_fill()
{
  echo $@
  (
  cat $@ | grep dttadj | grep fil
  ) > $tmp1
  sh sh/dtgplot1.sh $dest:adj_fill adj $tmp1
}
# ----------------------------------------------------------------------------
dir=./timing
pc1=alpha33
pc2=dell66
sparc1=sun4150
sparc2=sun4110
d1=$dir/$pc1/d950212.tim
tmp1=/tmp/dtgplots.tmp1

x standard QNX_Dell_vs_Alpha \
  $dir/$pc1/q950213.tim \
  $dir/$pc2/q950213.tim

x standard SUN_150_vs_110 \
  $dir/$sparc1/s950324.tim \
  $dir/$sparc2/s950325.tim

x standard qw0_vs_qw1 \
  $dir/i66award/cache.tim \
  $dir/i66award/nocache.tim \
  $dir/i50ami/cache.tim

  adj_method /tmp/timing.out

x sh sh/dtgplot1.sh $dest:adj   adj   /tmp/timing.out

x sh sh/dtgplot1.sh $dest:adj   adj_runs \
x   /usr/erb/prj/dt/timing/old.out \
x   /usr/erb/prj/dt/timing/old2.out

x adj_fill /tmp/timing.out

x standard qnx432_vs_qnx416 \
  $dir/dell66/q950717.tim

# grep QNX32 $q1 $q2 >  $tmp1
# grep DOS32 $d1     >> $tmp1
# cat $s1            >> $tmp1
# dtgplot1.sh $dest:32bit_cast  cast $tmp1
# dtgplot1.sh $dest:32bit_log   log  $tmp1
