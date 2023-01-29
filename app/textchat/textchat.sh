
script=textchat.sh

  ipc=rocket,0:9001						# our server ipc
  path=rocket,0:700  					# streaming ipc spec sans seq
  audio1=rocket,0:6033
  audio2=rocket,0:6044

ident1=AUD33
ident2=AUD44
out=/tmp/textchat.out
grep=/tmp/textchat.grep
dialog=text,curses
dialog=none
graphics=graphics
config=textchat.cfg

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
_run()
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
_rungrep()
{
  __cmd="$1"
  __grep="$2"
  _sayq $__cmd
  rm -f $grep
  eval "$__cmd -errto $grep -dbgto $grep"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$__cmd\""
  fi
  grep -i "$__grep" $grep
#  rm -f $grep
}
# -----------------------------------------------------------------------------
_gone()
{
  sleep 1
  ps -ax | 								\
  grep "textchat -op $1" |				\
  grep -v "grep" |						\
  grep "textchat -op $1" >/dev/null	  	# see if server is gone
  rc=$?
  if [ $rc -ne 1 ]
  then
    _say "textchat -op $1 still alive!"
  fi
}
# -----------------------------------------------------------------------------
_xterm()
{
  rm -f $out
  cmd="xterm \
    -geometry 160x24+1+60 \
    -fg MistyRose \
    -bg DarkSlateGray \
    -cr Red \
    -ms Red \
    -sb \
    -sl 2048 \
    -e textchat.sh foreground"
  _sayq "$cmd"
  $cmd &
}
# -----------------------------------------------------------------------------
_foreground()
{
  rm -f $out
  cmd="textchat \
    -op gui \
    -connect $ipc \
    -ident $ident1 \
    -config $config \
    -dialog $dialog \
    -D usr1+async+command \
    -errto $out \
    -dbgto $out \
    -sayto $out"
  _sayq "$cmd"
  $cmd 
  cat < $out
  read answer
}
# -----------------------------------------------------------------------------
_graphics()
{
  rm -f $out
  cmd="textchat \
    -op dialog \
    -connect $ipc \
    -ident $ident1 \
    -path ${path}%d \
    -dialog $graphics \
    -dt 100 \
    -D usr1+async+command+alimits+ipc"
  _sayq $cmd
  $cmd 
}
# -----------------------------------------------------------------------------
_command()
{
  __cmd="$1"
  _rungrep "uclient \
    -connect $ipc \
    -input \"$__cmd\" \
    -D reply" \
    "state:\|error:\|rc:\|result:"
}
# -----------------------------------------------------------------------------
_state()
{
  _say "$ipc state:"
  _rungrep "uclient \
    -connect $ipc \
    -input state \
    -D reply" \
    "state:\|error:\|rc:\|result:"
  _say "$audio1 state:"
  _rungrep "uclient \
    -connect $audio1 \
    -input state \
    -D reply" \
    "state:\|error:\|rc:\|result:"
  _say "$audio2 state:"
  _rungrep "uclient \
    -connect $audio2 \
    -input state \
    -D reply" \
    "state:\|error:\|rc:\|result:"
}
# -----------------------------------------------------------------------------
_exit()
{
  _run uclient \
    -connect $ipc \
    -input exit \
    -D reply 2>&1 | \
  grep "state:\|error:\|rc:\|result:"
}
# -----------------------------------------------------------------------------
_check()
{
  __ipc="$1"
  __field="$2"
  __expect="$3"
  rm -f $grep
  uclient -connect $__ipc -input state -D reply -dbgto $grep
  __result=`grep "$__field" < $grep`
  echo "$__result" | grep "$__expect" > /dev/null
  rc=$?
  if [ $rc -eq 0 ]
  then
    _say "verified $__result"
  else
    _say "NOT verified $__result"
  fi
}
# -----------------------------------------------------------------------------
_uclient()
{
  __ipc="$1"
  __input="$2"
  rm -f $grep
  uclient -connect $__ipc -input $__input -D reply -dbgto $grep
  __result=`grep "server_error\|server_rc: -1" < $grep`
  rc=$?
  if [ $rc -eq 0 ]
  then
    _say "BAD result:" `echo $__result | head -1`
  fi
}
# -----------------------------------------------------------------------------
_check_talking()
{
  _check $ipc server_state "server_state: T"
  _check $audio1 istream_spec audio,fake
  _check $audio1 ostream_spec $ipc3
  _check $audio2 istream_spec $ipc3
  _check $audio2 ostream_spec audio,fake
}
# -----------------------------------------------------------------------------
_check_listening()
{
  _check $ipc server_state "server_state: L"
  _check $audio1 istream_spec $ipc3
  _check $audio1 ostream_spec audio,fake
  _check $audio2 istream_spec audio,fake
  _check $audio2 ostream_spec $ipc3
}
# -----------------------------------------------------------------------------
_check_idle()
{
  _check $ipc server_state "server_state: I"
  _check $audio1 state waiting
  _check $audio2 state waiting
}
# -----------------------------------------------------------------------------
_converse()
{
  _xterm
  sleep 1
  _uclient $ipc partner\?$ident2

  _uclient $ipc talk
  sleep 2
  _check_talking

  _uclient $ipc listen
  sleep 2
  _check_listening

  _uclient $ipc talk
  sleep 2
  _check_talking

  _uclient $ipc listen
  sleep 2
  _check_listening

  _uclient $ipc idle
  sleep 3
  _check_idle

  _uclient $ipc exit
}
  
# -----------------------------------------------------------------------------

cmd=${1:-all}
shift 

_${cmd} $@
