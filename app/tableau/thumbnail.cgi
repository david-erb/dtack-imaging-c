#!/bin/sh
echo Content-type: text/plain
echo

exec 2>&1				# catch stderr too


# ------------------------------------------------------------
_say()
{
  echo "env.cgi: $@" 
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit
}
# ------------------------------------------------------------
_run()
{
  _say "$@" 1>&2
  eval "$@"
  if [ $? -ne 0 ]
  then
    _error "doing \"$@\""
  fi
}
# ------------------------------------------------------------
# ------------------------------------------------------------

./thumbnail.sh jpeg/gryphon1 cam0 rgb888
./thumbnail.sh jpeg/cal1 cam0 luminance
./thumbnail.sh jpeg/cal1 cam1 luminance
./thumbnail.sh jpeg/cal1 cam2 luminance
./thumbnail.sh jpeg/cal1 cam3 luminance

exit
