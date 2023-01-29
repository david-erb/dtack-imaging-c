# script to run simple ustream tests with the softwar<e decoder

ipctype=socket
case x$1 in
  "x-qbaby") shift; ipctype=qbaby;;
esac

script=dttipc.sh

#
# -----------------------------------------------------------------------------
_say()
{
  echo "$script: $@" 
}
#
# -----------------------------------------------------------------------------
_doing()
{
  _say "          $@" 
}
# -----------------------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit
}
# -----------------------------------------------------------------------------
_run()
{
  if [ -n "$DT_SH" ]
  then
    _say "$@"
  fi
  eval "$@"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$@\""
  fi
}

# --------------------------------------------------------------------------------
_do1()
{
  _run "$@"
}

# --------------------------------------------------------------------------------
_all()
{

if [ $ipctype = qbaby ]
then
  _do1 dttlow ipc -var 1                  qbaby,xxx
  _do1 dttlow ipc -var wto      -D ass    qbaby,xxx
  _do1 dttlow ipc -var fto      -D ass    qbaby,xxx
  _do1 dttlow ipc -var cto      -D ass    qbaby,xxx
  _do1 dttlow ipc -var rto      -D ass    qbaby,xxx
  _do1 dttlow ipc -var pto      -D ass    qbaby,xxx
  _do1 dttlow ipc -var ssuicide -D ass    qbaby,xxx
  _do1 dttlow ipc -var csuicde  -D ass    qbaby,xxx
else
  _do1 dttlow ipc -var 1
fi
}
# --------------------------------------------------------------------------------

if [ $# -eq 1 ]
then
  cmd=$1
  shift
else
  cmd=all
fi

_${cmd} $@

