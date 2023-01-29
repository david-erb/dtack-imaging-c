script=tabgrab.sh

exe=tabgrab
ipaddr=`hostname`
moist=moist.tableau.com
server_port=5720

# -----------------------------------------------------------------------------
_say()
{
  echo "$script: $@" 
}
# -----------------------------------------------------------------------------
_sayn()
{
  echo -n "$script: $@" 
}
# -----------------------------------------------------------------------------
_sayq()
{
  if [ -n "$DT_SH" ]
  then
    echo "$script: $@" 
  fi
}
# -----------------------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit
}
# -----------------------------------------------------------------------------
_usage()
{
  _say "usage: $@"
  exit
}
# -----------------------------------------------------------------------------
_argcheck()
{
  __have=$1
  __minwant=$2
  __maxwant=$3
  __usage=$4
  if [ $__have -lt $__minwant ]
  then
    _usage "$__usage"
  fi
  if [ $__have -gt $__maxwant ]
  then
    _usage "$__usage"
  fi
}
# -----------------------------------------------------------------------------
_runfg()
{
  _sayq "$@"
  eval "$@"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$@\""
  fi
}
# -----------------------------------------------------------------------------
_runbg()
{
  _sayq "$@"
  $@ &
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$@\""
  fi
}
# -----------------------------------------------------------------------------
_meteor()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
     -connect rocket,$ipaddr:$server_port \
     -input meteor,/dev/meteor0,320,240 \
     -output //j/d/tabgrab.dat \
     -omega 1000 \
     -D usr1+usr2+command \
     -errto stdout \
     -dbgto stdout \
     -sayto stdout"
}
# -----------------------------------------------------------------------------

_argcheck $# 1 99 "$cmd subcommand"

cmd=${1:-all}

shift 

_${cmd} $@
