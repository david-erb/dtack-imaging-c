# script to run simple ustream tests with the softwar<e decoder

script=ustream.sh

ipc1=rocket,0:6011
stream1=ipc,rocket,0:7011
ident1=AUD11

ipc2=rocket,0:6022
stream2=ipc,rocket,0:7022
ident2=AUD22

ipc3=rocket,0:6033
stream3=ipc,rocket,0:7033
ident3=AUD33

fake=audio,fake 						# fake audio source/sink
audio=audio,sbqnx,/dev/dsp				# real audio source/sink

testdir=/tmp							# where to write test files
tmp1=$testdir/ustream1.dat				# short test filename
tmp2=$testdir/ustream2.dat				# short test filename
tmpstate=$testdir/ustream.state		    # tmp output file

houston=/devel/test/uaudio/houston.pcm	# Houston audio clip
nul=/dev/null

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
# -----------------------------------------------------------------------------
_generate()
{
  xfile=$1
  xlines=${2:-100}
  xn=0
  while [ $xn -lt $xlines ]
  do
    echo "01234567890123456789012345678901234567890123456789"
    let xn=$xn+1
  done > $xfile
}
# -----------------------------------------------------------------------------
_verify_files()
{
  cmp -s $1 $2							# verify the file
  if [ $? -ne 0 ]
  then
    _say "NOT verified: $1 and $2 differ!"
  else
    _say "verified: file contents"
  fi
}

# -----------------------------------------------------------------------------
_verify_string()
{
  __state="$1"
  shift
  __expect="$1"
  shift
  echo "$__state" | \
  grep -i "$__expect" > $nul
  if [ $? -ne 0 ]
  then
    _say "NOT verified:$__state"
  else
    _say "verified:$__state"
  fi
}
# -----------------------------------------------------------------------------
_server()
{
  __connect=$1
  shift
  __ident=$1
  shift
  __istream=$1
  shift
  server="ustream						\
    -op server							\
    -connect $__connect	 				\
    -ident $__ident						\
    -path $__istream					\
    -errto stdout -dbgto stdout $@"
  if [ -n "$DT_SH" ]
  then
    _say $server
  fi
  $server &
}
# -----------------------------------------------------------------------------
_state()
{
  __ipc=$1
  shift

  _run uclient 							\
    -connect $__ipc		            	\
    -input "state" -D reply 			\
    -dbgto stdout >$tmpstate			# ask server for it's state

  state=`grep -i "server_state:" $tmpstate | cut -d: -f3-`

  error=`grep -i "last_error:" $tmpstate | cut -d: -f3-`

  rm -f $tmpstate
}
# -----------------------------------------------------------------------------
_cease()
{
  __ipc=$1
  _run uclient 							\
    -connect $__ipc						\
    -input "cease"
  _state $__ipc
  _verify_string "$state" "waiting for command"
  _verify_string "$error" "none"
}
# -----------------------------------------------------------------------------
_terminate()
{
  __connect=$1
  shift
  _run uclient 							\
    -connect $__connect					\
    -input "exit" $@
  sleep 1
  ps -ax | 								\
  grep -- "-connect $__connect" |		\
  grep -v "grep" |						\
  grep -- "-connect $__connect" >$nul	# see if server is gone
  rc=$?
  if [ $rc -ne 1 ]
  then
    _say "$__connect still alive!"
  else
    _say "verified: $__connect successfully exited"
  fi
}
# -----------------------------------------------------------------------------
# run the ustream server in the background, then the client
# client tells server to copy from one file to another

_copy()
{
  rm -f $tmp1 $tmp2
  _generate $tmp1 100				 	# generate test file
  _doing "running server"
  _server $ipc1 $ident1 none 			# run the server
  sleep 1

  _run uclient 							\
    -connect $ipc1		 				\
    -input "stream\?file,$tmp1\&file,$tmp2"

  sleep 1								# wait for transfer to happen

  _doing "terminating server"
  _terminate $ipc1							# make sure server is gone
  
  _verify_files $tmp1 $tmp2				# verify the file

  rm -f $tmp1 $tmp2
}
# -----------------------------------------------------------------------------
# run two ustream servers in the background
# tell second server to copy from first server to file
# tell first server to copy from file to other server

_ipc()
{
  rm -f $tmp1 $tmp2

  _generate $tmp1 100				 	# generate test file
  _doing "running first server"
  _server $ipc1 $ident1 $stream1		# run the first server
  _doing "running second server"
  _server $ipc2 $ident2 $stream2		# run the second server
  sleep 1

  _doing "starting transfer"

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc\&file,$tmp2\&$ident1"

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?file,$tmp1\&$stream2\&$ident2"

  sleep 1								# wait for transfer to actually happen

  _doing "terminating first server"
  _terminate $ipc1						# make sure first server is gone
  _doing "terminating second server"
  _terminate $ipc2 						# make sure second server is gone

  _verify_files $tmp1 $tmp2				# verify the file

  rm -f $tmp1 $tmp2
}
# -----------------------------------------------------------------------------
# run two ustream servers in the background
# try to tell the two servers to stream, but give wrong idents

_reject()
{
  rm -f $tmp1 $tmp2

  _generate $tmp1 100				 	# generate test file
  _doing "running first server"
  _server $ipc1 $ident1 $stream1		# run the first server
  _doing "running second server"
  _server $ipc2 $ident2 $stream2		# run the second server
  sleep 1
										# ....................................
  _doing "starting first transfer"

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc\&file,$tmp2\&xxx"

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?file,$tmp1\&$stream2\&$ident2"
										# ....................................
  _state $ipc1							# ask first server how it's doing

  _verify_string "$state" "waiting for command"
  _verify_string "$error" "nacked"
										# ....................................
  _state $ipc2							# ask second server how it's doing

  _verify_string "$state" "waiting for command"
  _verify_string "$error" "did not connect"
										# ....................................
  _doing "starting second transfer"

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc\&file,$tmp2\&$ident1"

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?file,$tmp1\&$stream2\&xxx"
  sleep 1
										# ....................................
  _state $ipc1							# ask first server how it's doing

  _verify_string "$state" "waiting for command"
  _verify_string "$error" "not permitted"
										# ....................................
  _state $ipc2							# ask second server how it's doing

  _verify_string "$state" "waiting for command"
  _verify_string "$error" ""
										# ....................................

  _doing "terminating first server"
  _terminate $ipc1							# make sure first server is gone
  _doing "terminating second server"
  _terminate $ipc2 							# make sure second server is gone

  rm -f $tmp1 $tmp2
}
# -----------------------------------------------------------------------------
# simulate server with locking on

_play_locked()
{
  _doing "running server"
  _server $ipc1 $ident1 $stream1		# run the server
  sleep 1

  _run uclient 							\
    -connect $ipc1						\
    -input "lock"						# tell server go to locked mode

  sleep 1								# short time before asking

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?ipc,$ipc3\&$fake"	# ask server to play

  _state $ipc1
  _verify_string "$state" "locked, waiting for command"
  _verify_string "$error" "because locked"

  _doing "terminating server"
  _terminate $ipc1							# make sure first server is gone
}
# -----------------------------------------------------------------------------
# simulate server told to play but never gets connect from client

_play_timeout()
{
  _doing "running server"
  _server $ipc1 $ident1 $stream1		# run the server
  sleep 1

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?ipc,$ipc3\&$fake"	# tell server to play

  sleep 2								# short time before asking

  _state $ipc1

  _verify_string "$state" "waiting for command"
  _verify_string "$error" "did not connect"

  _doing "terminating server"
  _terminate $ipc1							# make sure first server is gone
}
# -----------------------------------------------------------------------------
# run two ustream servers in the background
# tell second server to copy from first server to fake audio
# tell first server to copy from fake audio to other server
# try to tell each server to do something different

_busy()
{
  _doing "running first server"
  _server $ipc1 $ident1 $stream1		# run the first server
  _doing "running second server"
  _server $ipc2 $ident2 $stream2 		# run the second server
  sleep 1

  _doing "starting transfer"

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc\&$fake\&$ident1"

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?$fake\&$stream2\&$ident2"
										# ....................................
  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?$fake\&$stream3\&$ident3"
  _state $ipc1
  _verify_string "$state" "shoveling"
  _verify_string "$error" "busy"
										# ....................................
  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?$fake\&$stream3\&$ident3"
  _state $ipc2
  _verify_string "$state" "shoveling"
  _verify_string "$error" "busy"
										# ....................................
  _doing "ceasing first server"
  _cease $ipc1							# cease first server
  _state $ipc2							# make sure second server ceased
  _verify_string "$state" "waiting for command"
  _verify_string "$error" "busy"
										# ....................................
  _doing "terminating first server"
  _terminate $ipc1						# make sure first server is gone
  _doing "terminating second server"
  _terminate $ipc2 						# make sure second server is gone
}
# -----------------------------------------------------------------------------
# run two ustream servers in the background
# tell second server to copy from first server to fake audio
# tell first server to copy from fake audio to other server
# tell servers to switch to file copying

_reverse()
{
  rm -f $tmp1 $tmp2

  _doing "running first server"
  _server $ipc1 $ident1 $stream1		# run the first server
  _doing "running second server"
  _server $ipc2 $ident2 $stream2		# run the second server
  sleep 1

  _doing "starting fake audio transfer"

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc\&$fake\&$ident1"

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?$fake\&$stream2\&$ident2"
										# ....................................
  sleep 1

  _state $ipc1
  _verify_string "$state" "shoveling"
  _verify_string "$error" "none"

  _state $ipc2
  _verify_string "$state" "shoveling"
  _verify_string "$error" "none"
										# ....................................
  _generate $tmp1 100				 	# generate test file
  _doing "starting file transfer"

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc\&file,$tmp2\&$ident1"

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?file,$tmp1\&$stream2\&$ident2"

  sleep 1								# wait for transfer to actually happen
										# ....................................
  _run uclient 							\
    -connect $ipc1						\
    -input "cease"
  _state $ipc1
  _verify_string "$state" "waiting for command"
  _verify_string "$error" "not currently shoveling"

  _run uclient 							\
    -connect $ipc2						\
    -input "cease"
  _state $ipc2
  _verify_string "$state" "waiting for command"
  _verify_string "$error" "not currently shoveling"

										# ....................................
  _doing "terminating first server"
  _terminate $ipc1						# make sure first server is gone
  _doing "terminating second server"
  _terminate $ipc2 						# make sure second server is gone

  _verify_files $tmp1 $tmp2				# verify the file

  rm -f $tmp1 $tmp2
}
# -----------------------------------------------------------------------------
_timing()
{
  rm -f $tmp1 $tmp2

  _doing "running first server"
  _server $ipc1 $ident1 $stream1		# run the first server
  _doing "running second server"
  _server $ipc2 $ident2 $stream2		# run the second server
  sleep 1

  _doing "starting timing"

  _run uclient 							\
    -connect $ipc1						\
    -input "state"						\
    -reps 100 -D time
										# ....................................
  _doing "terminating first server"
  _terminate $ipc1						# make sure first server is gone
  _doing "terminating second server"
  _terminate $ipc2 						# make sure second server is gone
}
# -----------------------------------------------------------------------------
# play audio file

_play_file()
{
  _doing "running server"
  _server $ipc1 $ident1 -D usr1+pixel					# run the server
  sleep 1

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?file,$houston\&$audio"

  sleep 3
  _doing "terminating server"
  _terminate $ipc1						# make sure server is gone
}
# -----------------------------------------------------------------------------
# simulate audio with two ustream servers
# no way to verify data

_audio()
{
  _doing "running first server"
  _server $ipc1 $ident1					# run the first server
  _doing "running second server"
  _server $ipc2 $ident2					# run the second server
  sleep 2

  _run uclient 							\
    -connect $ipc2						\
    -input "stream\?ipc,$ipc3\&$fake"	# tell one server to play

  _run uclient 							\
    -connect $ipc1						\
    -input "stream\?$fake\&ipc,$ipc3"	# tell other server to record

  xn=0
  while [ $xn -lt 5 ]					# poll it while it works
  do
    sleep 5								# short time between polling

    _run uclient 						\
      -connect $ipc1					\
      -input "state" -D reply 			\
      -dbgto stdout |					\
      grep -i "server_state:"			# ask first server how it's doing
    _run uclient 						\
      -connect $ipc2					\
      -input "state" -D reply 			\
      -dbgto stdout |					\
      grep -i "server_state:"			# ask second server how it's doing

    let xn=$xn+1
  done

  _doing "terminating first server"
  _terminate $ipc1						# make sure first server is gone
  _doing "terminating second server"
  _terminate $ipc2						# make sure second server is gone
}

# --------------------------------------------------------------------------------
_do1()
{
  _doing "******** $@"
  _run "_$@"
}

# --------------------------------------------------------------------------------
_all()
{
  _do1 copy
  _do1 ipc
  _do1 reject
  _do1 play_locked
  _do1 play_timeout
  _do1 busy
  _do1 reverse
}
# --------------------------------------------------------------------------------

cmd=${1:-all}
shift

_${cmd} $@

