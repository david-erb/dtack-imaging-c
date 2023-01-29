#! /bin/bash

script=tabstart.sh

# -----------------------------------------------------------------------------
_say()
{
  logger -t "$script[$$]" -p daemon.info "$@"
}
# -----------------------------------------------------------------------------
_sayq()
{
  if [ -n "$DT_SH" ]
  then
    _say "$@" 
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
_check1()
{
  ps -ax | grep -v grep | grep "$1" >/dev/null
  rc=$?
}

# -----------------------------------------------------------------------------
_checklist()
{
(
  while read command args
  do
    _check1 "$command $args"		# see if this process is running
    if [ $rc -ne 0 ]			# process not running?
    then
      _say "starting $command $args"
      _runbg $command $args
      sleep 2s
    else				# process already running?
      if [ $first -ne 0 ]
      then
        _say "monitoring $command $args"
      fi
    fi
  done 
) < $argfile 
}
# -----------------------------------------------------------------------------

dev=/dev/modem

if [ $# -ne 1 ]
then
  _error "no argfile given"
fi
argfile="$1"
shift

first=1
while true				# loop forever
do
  _checklist				# check list of processes to run
  first=0				# don't say "monitoring" again
  sleep 10s				# delay before checking again
done
