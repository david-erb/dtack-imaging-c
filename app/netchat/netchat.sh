
script=netchat.sh

config=/CONFIG/ustate.cfg

out=/tmp/netchat.sh.$$

test_control=auto						# default test control

ustream_dbg=usr1						# debug level for stream shoveler
netchat_dbg=usr1						# debug level for user interface

(sin -Pqbaby | grep qbaby) >/dev/null 2>&1
if [ $? -ne 0 ]
then
  qbaby dt/qbaby &
fi

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
_run_and_pause()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"
  rm -f $out
  netchat 								\
    -ident $__ident 					\
    -config $config 					\
    -dialog text,curses					\
    -D usr1+command						\
    -errto $out -dbgto $out -sayto $out
  cat < $out
  read answer
  rm -f $out
}
# -----------------------------------------------------------------------------
_xterm()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  let __row="(${__position}-1)*180"
  _runbg xterm 							\
    -geometry 140x12+1+${__row}			\
    -fg MistyRose 						\
    -bg DarkSlateGray 					\
    -cr Red 							\
    -ms Red 							\
    -sb 								\
    -sl 2048 							\
    -e netchat.sh run_and_pause $__ident
}
# -----------------------------------------------------------------------------
_graphics()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  let __row="(${__position}-1)*130"
 
  _runbg netchat						\
    -ident $__ident 					\
    -config $config 					\
    -dialog graphics 					\
    -x 0 -y $__row						\
    -D $netchat_dbg
}
# -----------------------------------------------------------------------------
_slaughter()
{
  __regexp="$1"

  ps -ax | grep "$__regexp" | grep -v grep | grep -v $script > $out

  for __pid in `awk '{print $1}' < $out`
  do
    _runfg "kill -9 $__pid"
  done

  rm -f $out
}
# -----------------------------------------------------------------------------
_uclient()
{
  __ident=${1:-AUD11}
  __input=${2:-state}
  __server=${3:-netchat}
  __server_rc=${4:-0}
  __server_error=${5:-none}
  _argcheck $# 2 5 "$cmd ident input [server server_rc]]"

  __cmd="ustate -config $config \
    $__ident $__server dgserver_spec default"
  _sayq $__cmd
  __ipc=`$__cmd 2>/dev/null`
  if [ -z "$__ipc" -o "$_ipc" = "default" ]
  then
    _error "ustate failed"
  fi

  rm -f $out

  if [ $__input = "state" ]
  then
    uclient -connect $__ipc -input $__input -D reply -dbgto $out -dt 5000
    return
  fi

  if [ $test_control = "tell_qc" ]
  then
    _sayn "now click $__ident $__input...hit enter to continue"
    read answer
  else
    if [ $test_control = "pause_first" ]
    then
      _sayn "telling $__ident ($__ipc) \"$__input\"...hit enter to continue"
      read answer
    fi
    uclient -connect $__ipc -input $__input -D reply -dbgto $out -dt 5000
    if [ $__server_rc -ne 2 ]
    then
      _check_for server_rc "$__server_rc"
      if [ $__server_rc -ne 0 ]
      then
        _check_for server_error "$__server_error"
      fi
    fi
  fi
}
# -----------------------------------------------------------------------------
_check_for()
{
  __field=${1:-server_error}
  __expect=${2:-0}
  _argcheck $# 2 2 "$cmd field expect"

  __result=`grep -- "$__field:" < $out | head -1 | sed 's/uclient: //'`
  echo "$__result" | grep -- "$__expect" > /dev/null
  rc=$?
  if [ $rc -ne 0 ]
  then
    _say "BAD result:" `echo $__result | head -1`
  else
    if [ "$__expect" != "0" ]			# boring to repeat this
    then
      _say "verified: $__result"
    fi
  fi
}
# -----------------------------------------------------------------------------
_check_not()
{
  __field=${1:-server_error}
  __expect=${2:-0}
  _argcheck $# 2 2 "$cmd field expect"

  __result=`grep -- "$__field:" < $out | head -1 | sed 's/uclient: //'`
  echo "$__result" | grep -- "$__expect" > /dev/null
  rc=$?
  if [ $rc -eq 0 ]
  then
    _say "BAD result:" `echo $__result | head -1`
  else
    if [ "$__expect" != "0" ]			# boring to repeat this
    then
      _say "verified: not $__expect"
    fi
  fi
}
# -----------------------------------------------------------------------------
_auto1()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"

  _uclient $__ident "partner?$__ident" netchat -1 "can't talk to self"
  
  _uclient $__ident "partner?xxxxxx" netchat -1 "can't find xxxxxx"
  
  _uclient $__ident "talk" netchat -1 "choose partner first"
  
  _uclient $__ident "partner?AUD19" netchat -1 "AUD19 is offline"
  
  rm -f $out
}
# -----------------------------------------------------------------------------
_auto2()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  _argcheck $# 2 2 "$cmd ident1 ident2"

  _uclient $__ident1 "partner?$__ident2"

										# ----------------------------------
  _uclient $__ident1 "talk"				# start #1 talking

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_state "state: T"
  
  _uclient $__ident2 "state"
  _check_for server_state "state: L"
										# ----------------------------------
  _uclient $__ident2 "talk"				# reverse at #2

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_state "state: L"
  
  _uclient $__ident2 "state"
  _check_for server_state "state: T"
										# ----------------------------------
  _uclient $__ident2 "privacy"			# set #2 into privacy

  sleep 1
  _uclient $__ident1 "state"
  _check_for server_state "state: I"

  _uclient $__ident1 "talk"	netchat -1 "privacy"

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_state "state: I"	# #1 can't talk, stays idle
  
  _uclient $__ident2 "state" 
  _check_for server_state "state: P"

  _uclient $__ident2 "state" ustream
  _check_for last_error "$ident1 call rejected"

  _uclient $__ident2 "privacy"			# set #2 out of privacy
  
  _uclient $__ident2 "state"
  _check_for server_state "state: I"
  
  rm -f $out
}
# -----------------------------------------------------------------------------
_auto3()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  __ident3=${3:-AUD13}
  _argcheck $# 3 3 "$cmd ident1 ident2 ident3"

  _uclient $__ident1 "partner?$__ident2"

										# ----------------------------------
  _uclient $__ident1 "talk"				# start #1 talking to #2

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_state "state: T"
  
  _uclient $__ident2 "state"
  _check_for server_state "state: L"
										# ----------------------------------
  _uclient $__ident3 "partner?$__ident1"

  _uclient $__ident3 "talk" netchat -1 "busy"

  _uclient $__ident3 "partner?$__ident2"

  _uclient $__ident3 "talk" netchat -1 "busy"

										# ----------------------------------
  _uclient $__ident1 "talk"				# stop #1 and #2 talking

  _uclient $__ident1 "state"
  _check_for server_state "state: I"

  _uclient $__ident2 "state"
  _check_for server_state "state: I"

  rm -f $out
}
# -----------------------------------------------------------------------------
_blast()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  _argcheck $# 2 2 "$cmd ident1 ident2"

  _uclient $__ident1 "partner?$__ident2"

  while true
  do
    _uclient $__ident1 "talk" netchat 2

	__result=`grep "server_rc:" $out`
    echo "$__result" | grep ": 0" > /dev/null
    __rc=$?

    if [ $__rc -ne 0 ]				   	# did not start?
    then
      _check_for server_error "busy"	# should be busy
    else
      __sleep=`date +%S`
      let __sleep=$__sleep%5+2
	  _say "talking for $__sleep"
      sleep $__sleep
      _uclient $__ident1 "state" ustream	
      _check_for istream_spec "audio"
      _check_for permit "$__ident2"
      _uclient $__ident2 "state" ustream
      _check_for ostream_spec "audio"
      _check_for permit "$__ident1"
	  _uclient $__ident1 "idle"
	  _check_for server_rc "0"
	  sleep 1
	  _uclient $__ident1 "state"		
	  _check_for server_state ": I"		# talker idle
	  _uclient $__ident2 "state"		
      _check_not permit "$__ident1"		# listener not listening to us
    fi

  done

  rm -f $out
}
# -----------------------------------------------------------------------------
_state()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"

  _uclient $__ident "state" ustream 
  cat < $out
  _uclient $__ident "state" netchat
  cat < $out
  _say `grep server_state $out | sed 's/uclient: //'`

  rm -f $out
}
# -----------------------------------------------------------------------------
_state3()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  __ident3=${3:-AUD13}
  _argcheck $# 3 3 "$cmd ident1 ident2 ident3"

  _uclient $__ident1 "state" ustream
  _say `grep server_state $out | sed 's/uclient: //'`
  _uclient $__ident2 "state" ustream
  _say `grep server_state $out | sed 's/uclient: //'`
  _uclient $__ident3 "state" ustream
  _say `grep server_state $out | sed 's/uclient: //'`

  rm -f $out
}
# -----------------------------------------------------------------------------
_ustream()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"
  _runbg "ustream -config $config -ident $__ident -D $ustream_dbg"
}
# -----------------------------------------------------------------------------
_start_xterm()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  _xterm $__ident $__position
}
# -----------------------------------------------------------------------------
_start_graphics()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  _graphics $__ident $__position
}
# -----------------------------------------------------------------------------
# start given shoveler process
# should be done from sysinit so others can call in
_init()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"
  _ustream $__ident
}
# -----------------------------------------------------------------------------
_start()
{
  if [ -n "$NODE" ]
  then
    _start_graphics $@
  else
    _start_xterm $@
  fi
}
# -----------------------------------------------------------------------------
_clear()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"
  _slaughter "ident $__ident"
  today=`date +%y%m%d`
  rm -f /CONFIG/ustream.$__ident.$today
  rm -f /CONFIG/netchat.$__ident.$today
}
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
_restart()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  _clear $__ident
  _init $__ident						# start shoveler
  _start $__ident $__position			# start dialog
}
# -----------------------------------------------------------------------------
_restart2()
{
  _argcheck $# 0 0 "$cmd"
  _restart AUD11 1
  _restart AUD12 2
}
# -----------------------------------------------------------------------------
_restart3()
{
  _argcheck $# 0 0 "$cmd"
  _restart AUD11 1
  _restart AUD12 2
  _restart AUD13 3
}
# -----------------------------------------------------------------------------
_clear2()
{
  _argcheck $# 0 0 "$cmd"
  _clear AUD11 
  _clear AUD12 
}
# -----------------------------------------------------------------------------
_clear3()
{
  _argcheck $# 0 0 "$cmd"
  _clear AUD11 
  _clear AUD12 
  _clear AUD13 
}
# -----------------------------------------------------------------------------
_key1()
{
  test_control=pause_first
  _auto1 $@
}
# -----------------------------------------------------------------------------
_key2()
{
  test_control=pause_first
  _auto2 $@
}
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# tell qc person to do steps then hit enter, then verify server states
# presumes all netchats and ustreams are running and idle
_qc2()
{
  _argcheck $# 2 2 "$cmd ident1 ident2"
  test_control=tell_qc
  _auto2 $1 $2
}
# -----------------------------------------------------------------------------
_qc3()
{
  _argcheck $# 3 3 "$cmd ident1 ident2 ident3"
  test_control=tell_qc
  _auto3 $1 $2 $3
}
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
_node_init()
{
  __irupt=${1:-5}
  _argcheck $# 0 1 "$cmd [irupt]"
  slay -fQ Audio
  _sayn "starting Audio driver (irq $__irupt)"
  Audio sb -i $__irupt &
  echo -n "."
  date > /dev/null
  sleep 1
  echo -n "."
  date > /dev/null
  sleep 1
  echo -n "."
  date > /dev/null
  sleep 1
  echo -n "."
  date > /dev/null
  sleep 1
  echo "ok"
  _init AUD$NODE
}
# -----------------------------------------------------------------------------
_node_start()
{
  _argcheck $# 0 0 "$cmd no arguments"
  _start AUD$NODE
}
# -----------------------------------------------------------------------------
_node_restart()
{
  _argcheck $# 0 0 "$cmd no arguments"
  _restart AUD$NODE
}
# -----------------------------------------------------------------------------

if [ $# -lt 1 ]
then
  $script start
  $script restart
  $script clear
  exit
fi

cmd=${1:-all}
shift 

_${cmd} $@


#ifdef __USAGE
#/*..........................................................................
#| NAME
#| netchat.sh - netchat process starting and testing
#|
#| USAGE
#| usage: netchat.sh op [args]
#| op	operation (see below)
#| args	depend on operation
#| 
#| This script starts netchat from the sysinit or the custom menu.
#| It is also used for testing the netchat system.
#|
#| The operations and their arguments are:
#| node_init				start shoveler from sysinit 
#| node_start				start netchat dialog from custom menu 
#| node_restart				restart shoveler and dialog from custom menu 
#|
#| auto1 AUDxx				automated test a single dialog
#| auto2 AUDxx AUDyy		automated test two dialogs
#| auto3 AUDxx AUDyy AUDzz	automated test three dialogs
#|
#| qc2 AUDxx AUDyy			semi-automated test two dialogs
#| qc3 AUDxx AUDyy AUDzz	semi-automated test three dialogs
#| 
#| END
#*..........................................................................*/
#endif

