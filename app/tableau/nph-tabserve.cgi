#!/bin/sh

echo "HTTP/1.0 200"                     # give response header
echo "Content-type: text/plain"
echo

# nph-tabserve.cgi 
# author: David Erb erb@dtack.com
# written for: Tableau Web http://www.tableau.com
# Use this script any way you want.  Credit to the author would be nice.
#
# This is a cgi script for starting the Tableau server-side.
# A capture-side or browse-side applet hits this url first to get a server.
#
# Put this script in your cgi-bin directory.
# Set its execute permission on.
# Edit the "global things you should change".
#
# Test by hitting this URL from your browser:
#  http://your.host.com/nph-tabserve.cgi?ident=xxx
#

script=nph-tabserve.cgi

exec 2>> logs/$script.log

# ------------------------------------------------------------
# global things you should change:

ipaddr=`hostname`			# ipaddr where tabserve listens
jpeg=./jpeg                             # name of absolute part of jpeg path
bin=.                                   # path to executable

#
# ------------------------------------------------------------
# output line to debug stream
# this is error_log for most servers
# note this message is NOT in Common Log Format (but could be)
_say()
{
  echo "$script `date +%Y%m%d.%H%M.%S` [$$]: $@" 1>&2
}
#
# ------------------------------------------------------------
# emit a line of debug to debug string
# first argument to this function should be numeric debug level
# message is only printed if this argument is less or equal global debug
_debug()
{
  level=$1
  shift
  if [ $level -le $debug ]
  then
    _say "($level)" $@
  fi
}
# ------------------------------------------------------------
# emit error line to debug stream
_error()
{
  _debug $debug_error "ERROR $@ $REMOTE_HOST"
  echo "server_error: $@"
  exit
}
# ------------------------------------------------------------
# unsigned values to variables
_assign()
{
  variable="$1"
  shift
  eval "$variable=\"$@\""
}
# ------------------------------------------------------------
# slaughter process
_slaughter()
{
  __regexp="$1"
  # _debug $debug_cmd "slaughtering \"$1\""
  for __pid in `ps ax | \
                grep "$__regexp" | \
                grep -v grep | \
                grep -v $script | \
                awk '{print $1}'`
  do
    _debug $debug_cmd "kill -SIGINT $__pid"
    kill -SIGINT $__pid
  done
}
# ------------------------------------------------------------

debug_cmd=1                             # debug level for file logging
debug_error=0                           # debug level for error logging
debug_skip=1                            # debug level for file skip messages
debug_done=1                            # debug level for done message

q="$QUERY_STRING"
assigns=`echo $q | tr '&' ' '`          # separate the assignments
for assign in $assigns                  # for each assignment
do
  _assign `echo $assign | tr '=' ' '`   # make assignment
done

ipcspec=${ipcspec:-"$ipaddr:0"}       # new server's streaming ipc spec
max=${max:-"1000"}                      # timeout per incoming frame
debug=${debug:-"1"}
D=${D:-"usr1"}
dz=${dz:-"200"}

if [ -z "$ident" ]			# ident missing?
then
  _error "no ident given"
fi

dir="$jpeg/$ident"                      # directory where files live

if [ ! -d "$dir" ] 			# directory not found?
then
  _error "directory $dir does not exit"
fi

exe="$bin/02-03/tabserve -screen roll,$ident"
log=logs/tabserve.$ident.log

_slaughter "$exe"

if [ -n "$cmdport" ]
then
  cmdport="-cache http://$ipaddr:$cmdport"
fi

cmd="$exe \
  -connect rocket,$ipcspec $cmdport \
  -path $jpeg \
  -psi 50000 \
  -variant passthrough \
  -max $max \
  -dz $dz \
  -D $D \
  -errto $log \
  -dbgto $log \
  -sayto $log" 

_debug $debug_cmd $exe

export no_exit_on_failed_exec=1         # don't die if exec fails (bash only)

export LD_LIBRARY_PATH=$bin/lib:$LD_LIBRARY_PATH

exec $cmd                               # replace this shell with tableau

rc=$?                                   # exec failed?
_debug $debug_error "exec error $rc"

echo "server_error: exec error $rc"     

exit
