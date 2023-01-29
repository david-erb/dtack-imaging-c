
script=netchat.sh

config=/CONFIG/ustate.cfg

out=/tmp/netchat.sh.$$

dialog=text,curses						# dialog for non-graphics app
dialog=graphics							# dialog for QNX graphics app

# -----------------------------------------------------------------------------
_say()
{
  echo "$script: $@" 
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
  textchat 								\
    -ident $__ident 					\
    -config $config 					\
    -dialog $dialog 					\
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
  let __row="(${__position}-1)*100"
 
  _runbg textchat						\
    -ident $__ident 					\
    -config $config 					\
    -dialog $dialog 					\
    -x 0 -y $__row						\
    -D usr1+command
}
# -----------------------------------------------------------------------------
_slaughter()
{
  __regexp="$1"
  myslaughter "$__regexp"
}
# -----------------------------------------------------------------------------
_uclient()
{
  __ident=${1:-AUD11}
  __input=${2:-state}
  __server=${3:-netchat}
  _argcheck $# 2 3 "$cmd ident input"

  __cmd="ustate -config $config \
    $__ident $__server dgserver_spec default"
  _sayq $__cmd
  __ipc=`$__cmd 2>/dev/null`
  if [ -z "$__ipc" -o "$_ipc" = "default" ]
  then
    _error "ustate failed"
  fi

  rm -f $out
  uclient -connect $__ipc -input $__input -D reply -dbgto $out
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
_test1()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"

  _uclient $__ident "partner?$__ident"
  _check_for server_error "can't talk to self"
  _check_for server_rc "-1"
  
  _uclient $__ident "partner?xxxxxx"
  _check_for server_error "can't find xxxxxx"
  _check_for server_rc "-1"
  
  _uclient $__ident "talk"
  _check_for server_error "choose partner first"
  _check_for server_rc "-1"
  
  _uclient $__ident "partner?AUD19"
  _check_for server_error "AUD19 is offline"
  _check_for server_rc "-1"
  
  rm -f $out
}
# -----------------------------------------------------------------------------
_test2()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  _argcheck $# 2 2 "$cmd ident1 ident2"

  _uclient $__ident1 "partner?$__ident2"
  _check_for server_rc "0"

										# ----------------------------------
  _uclient $__ident1 "talk"				# start #1 talking
  _check_for server_rc "0"

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_rc "0"
  _check_for server_state "state: T"
  
  _uclient $__ident2 "state"
  _check_for server_rc "0"
  _check_for server_state "state: L"
										# ----------------------------------
  _uclient $__ident2 "talk"				# reverse at #2
  _check_for server_rc "0"

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_rc "0"
  _check_for server_state "state: L"
  
  _uclient $__ident2 "state"
  _check_for server_rc "0"
  _check_for server_state "state: T"
										# ----------------------------------
  _uclient $__ident2 "idle"				# idle at #2
  _check_for server_rc "0"

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_rc "0"
  _check_for server_state "state: I"
  
  _uclient $__ident2 "state"
  _check_for server_rc "0"
  _check_for server_state "state: I"
  
  rm -f $out
}
# -----------------------------------------------------------------------------
_test3()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  __ident3=${3:-AUD13}
  _argcheck $# 3 3 "$cmd ident1 ident2 ident3"

  _uclient $__ident1 "partner?$__ident2"
  _check_for server_rc "0"

										# ----------------------------------
  _uclient $__ident1 "talk"				# start #1 talking to #2
  _check_for server_rc "0"

  sleep 1

  _uclient $__ident1 "state"
  _check_for server_rc "0"
  _check_for server_state "state: T"
  
  _uclient $__ident2 "state"
  _check_for server_rc "0"
  _check_for server_state "state: L"
										# ----------------------------------
  _uclient $__ident3 "partner?$__ident1"
  _check_for server_rc "0"

  _uclient $__ident3 "talk"				# #3 gets busy trying #1
  _check_for server_error "busy"
  _check_for server_rc "-1"
										# ----------------------------------
  _uclient $__ident3 "partner?$__ident2"
  _check_for server_rc "0"

  _uclient $__ident3 "talk"				# #3 gets busy trying #2
  _check_for server_error "busy"
  _check_for server_rc "-1"

  rm -f $out
}
# -----------------------------------------------------------------------------
_blast()
{
  __ident1=${1:-AUD11}
  __ident2=${2:-AUD12}
  _argcheck $# 2 2 "$cmd ident1 ident2"

  _uclient $__ident1 "partner?$__ident2"
  _check_for server_rc "0"

  while true
  do
    _uclient $__ident1 "talk"

	__result=`grep "server_rc:" $out`
    echo "$__result" | grep ": 0" > /dev/null
    __rc=$?

    if [ $__rc -ne 0 ]				   	# did not start?
    then
      _check_for server_error "busy"	# should be busy
    else
      __sleep=`rand 5`
	  _sayq "talking for $__sleep"
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
_ustream()
{
  __ident=${1:-AUD11}
  _argcheck $# 1 1 "$cmd ident"
  _runbg "ustream -config $config -ident $__ident -D usr1+usr2+command+trace"
}
# -----------------------------------------------------------------------------
_start_xterm()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  _ustream $__ident
  _xterm $__ident $__position
}
# -----------------------------------------------------------------------------
_start_graphics()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  _ustream $__ident
  _graphics $__ident $__position
}
# -----------------------------------------------------------------------------
_start()
{
  _say $dialog
  if [ "$dialog" = "graphics" ]
  then
    _start_graphics $@
  else
    _start_xterm $@
  fi
}
# -----------------------------------------------------------------------------
_restart()
{
  __ident=${1:-AUD11}
  __position=${2:-1}
  _argcheck $# 1 2 "$cmd ident [position]"
  _clear $__ident
  _start $__ident $__position
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
_clear3()
{
  _argcheck $# 0 0 "$cmd"
  _clear AUD11 
  _clear AUD12 
  _clear AUD13 
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



