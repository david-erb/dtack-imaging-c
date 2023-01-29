
script="thumbnail.sh"

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

  _argcheck $# 3 3 "$script jpegdir cam color"

  dir=$1
  cam=$2
  color=$3
  exe=tab00-34/tableau

  rm -f $dir/ser.rol

  _runfg "$exe \
    -input ser,$cam.pat \
    -path $1 \
    -screen roll,. \
    -D usr2 \
    -op resize4 \
    -n 1 \
    -t 25 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"

  _runfg "mv -f $dir/ser.00000000.$color.enc $dir/$cam.thumbnail.jpg"


