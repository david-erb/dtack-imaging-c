args()
{
  arg1=$1
  arg2=$2
  arg3=$3
  arg4=$4
  arg5=$5
  arg6=$6
  arg7=$7
  arg8=$8
}
#================================================================
if test $# -lt 3
then
  echo "dtgplot1.sh: tims.sh: argument count wrong"
  exit 1
fi

dir=.
plot=$1
shift
type=$1
shift
files=$@
tmp1=/tmp/dtgplot1.tmp1
tmp2=/tmp/dtgplot1.tmp2
d=

case $type in
#=================================================================
  cast)
    grep dttcast $files >$tmp1
  ;;
  conv)
    grep dttconv $files >$tmp1
  ;;
  stack)
    grep dttstack $files >$tmp1
  ;;
  mem)
    grep dttmem $files >$tmp1
  ;;
  svga)
    grep fastg    $files > $tmp1
    grep svgac    $files >>$tmp1
  ;;
  log)
    grep "log.* unscaled"  $files > $tmp1
    grep "log.* scaled"    $files >>$tmp1
  ;;
  1d)
    grep dtt1d $files > $tmp1
  ;;
  adj)
    grep dttadj $files > $tmp1
  ;;
  adj_fill)
    grep dttadj $files | grep fill | grep -v white > $tmp1
  ;;
  adj_runs)
    grep dttadj $files | grep runs | grep -v white > $tmp1
  ;;
  adj_runf)
    grep dttadj $files | grep runf | grep -v white > $tmp1
  ;;
  all)
    cp $files $tmp1
  ;;
  *)
    echo "dtgplot1.sh: unknown type \"$type\""
    exit 1
  ;;
#=================================================================
esac

grep -v error $tmp1 > $tmp2

args `echo $plot | tr : ' '`
if [ "$arg1" = "ps" ]
then
  sh sh/gplot1.sh ps $tmp2
  echo "dtgplot1.sh: output to $dir/timing/ps/$arg2.ps"
  gnuplot /tmp/gplot1.gplot > $dir/timing/ps/$arg2.ps
fi

if [ "$arg1" = "dtview" ]
then
  sh sh/gplot1.sh pbm $tmp2
  echo "dtgplot1.sh: output to dtview, window $arg2"
  gnuplot /tmp/gplot1.gplot | dttool$d copy stdin :$arg2
fi

if [ "$arg1" = "aplot" ]
then
  sh sh/aplot1.sh 132 $tmp2
  mv /tmp/aplot1.aplot /tmp/$arg2.aplot
  echo "dtgplot1.sh: output to /tmp/$arg2.aplot"
fi
