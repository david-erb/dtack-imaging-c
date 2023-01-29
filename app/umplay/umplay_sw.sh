# script to run simple umplay tests with the software decoder
# -----------
# scenario 1: test software display
#   umplay_sw.sh show
# displays a short clip to the default display at 3 speeds: I, P and B
# comparing this to scenario 2 will indicate the display overhead
# success: visual
# -----------
# scenario 2: test software decode time
#   umplay_sw.sh time
# times decoding with no display of a short clip at 3 speeds: I, P and B
# success: times printed
# -----------
# scenario 3: test fifo server (like umplay with mpeg_decode in uniquant)
#   umplay_sw.sh fifo
# runs fifo server in background with some debug to console (not log file)
# runs client to stuff data and a series of stops, starts, and speed changes
# finally client tells server to exit and script checks this
# success: visual
# -----------
# scenario 4: test fifo server with pinging NOPs
#   umplay_sw.sh dec
# runs fifo server in background with some debug to console (not log file)
# runs one client to stuff data and a series of stops, starts, and speed changes
# runs another client to stuff a series of stops, starts, and speed changes
# finally client tells server to exit and script checks this
# success: visual
# -----------

script=umplay_sw.sh

ipc=rocket,0:7081						# server ipc
ipc_decdata=rocket,0:7089				# decdata test ipc

testdir=/devel/test/mpeg				# where test clips live
testclip1=nci9701a.mpg					# short test clip
testclip2=janice15.mpg					# long test clip

variant=ssg,raw,I

fifodir=/fifo							# where fifos are
cmdfifo=cmdfifo							# names of the fifos
decfifo=decfifo
repfifo=repfifo

datadir=/devel/test/umplay				# where data files live
stuff1=stuff1.dat						# simple stuff
stuff2=stuff2.dat						# loop stuff
stuff2e=stuff2e.dat						# just exit

deccmd1=deccmd1.dat						# simple deccmd
deccmd2=deccmd2.dat						# simple starve
deccmd3=deccmd3.dat						# loop deccmd
deccmd3e=deccmd3e.dat					# just exit

showscreen=
showkeyboard=trmios,,ctlkeys

noscreen=dummy
nokeyboard=none

#
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
_gone()
{
  sleep 1
  ps -ax | 								\
  grep "umplay -op $1" |				\
  grep -v "grep" |						\
  grep "umplay -op $1" >/dev/null	  	# see if server is gone
  rc=$?
  if [ $rc -ne 1 ]
  then
    _say "umplay -op $1 still alive!"
  fi
}

# -----------------------------------------------------------------------------
_time()
{
  _run time								\
  umplay 								\
    -op file							\
    -input $testdir/$testclip1 			\
    -variant ssg,raw,I,$noscreen 		\
    -keyboard $nokeyboard -D complex
  _run time								\
  umplay 								\
    -op file							\
    -input $testdir/$testclip1 			\
    -variant ssg,raw,P,$noscreen 		\
    -keyboard $nokeyboard -D complex
  _run time								\
  umplay 								\
    -op file							\
    -input $testdir/$testclip1 			\
    -variant ssg,raw,B,$noscreen		\
    -keyboard $nokeyboard -D complex
}
# -----------------------------------------------------------------------------
_show()
{
  _run time								\
  umplay 								\
    -op file							\
    -input $testdir/$testclip1 			\
    -variant ssg,raw,I,$showscreen 		\
    -keyboard $showkeyboard -D complex
  umplay 								\
    -op file							\
    -input $testdir/$testclip1 			\
    -variant ssg,raw,P,$showscreen 		\
    -keyboard $showkeyboard -D complex
  _run time								\
  umplay 								\
    -op file							\
    -input $testdir/$testclip1 			\
    -variant ssg,raw,B,$showscreen 		\
    -keyboard $showkeyboard -D complex
}
# -----------------------------------------------------------------------------
_fifo_prepare()
{
  rm -f $fifodir/$cmdfifo
  rm -f $fifodir/$decfifo
  rm -f $fifodir/$repfifo
  mkfifo $fifodir/$cmdfifo
  mkfifo $fifodir/$decfifo
  mkfifo $fifodir/$repfifo
  chmod a+rw $fifodir/$cmdfifo
  chmod a+rw $fifodir/$decfifo
  chmod a+rw $fifodir/$repfifo
}
# -----------------------------------------------------------------------------
# run the fifo server (this is how umplay runs with mpeg_decode in uniquant)

_fifo_server()
{
  server="umplay 						\
    -op fifo							\
    -connect $ipc						\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo \
    -variant $variant					\
    -D usr1+usr2+command				\
    -errto stdout -dbgto stdout"
  _sayq $server
  $server &
}
# -----------------------------------------------------------------------------
# send a series of stop, start, and speed change commands on the fifos

_fifo_client()
{
  _stuff=$1
  _run umplay 							\
    -op stuff							\
    -input $testdir/$testclip1 			\
    -config $datadir/$_stuff			\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo 
}
# -----------------------------------------------------------------------------
# run the fifo server in the background, then the stuff client
# the stuff client sends both data and commands on the fifos
# when client is done, check to make sure server has exited

_stuff()
{
  _fifo_server							# start server
  sleep 1
  _fifo_client $stuff1
  _gone fifo							# make sure server is gone
}

# -----------------------------------------------------------------------------
_deccmd()
{
  _deccmd=$1
  _run umplay 							\
    -op deccmd							\
    -connect $ipc_decdata				\
    -config $datadir/$_deccmd			\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo \
    -D command
}

# -----------------------------------------------------------------------------
_decdata()
{
  decdata="umplay						\
    -op decdata							\
    -connect $ipc_decdata				\
    -input $testdir/$testclip2 			\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo \
    -D command"
  _sayq $decdata
  $decdata &
}

# -----------------------------------------------------------------------------
# run the fifo server in the background, then the decdata and deccmd clients
# decdata writes data and deccmd writes commands
# when clients are done, check to make sure server has exited

_fifo_dec()
{
  _deccmd=$1
  _fifo_server							# start server
  sleep 1
  _decdata								# start data sender
  sleep 1
  _deccmd $_deccmd						# start command sender

  _gone fifo							# make sure server is gone
}

# -----------------------------------------------------------------------------
# 3-process model, simple commands

_normal()
{
  _fifo_dec $deccmd1
}

# -----------------------------------------------------------------------------
# 3-process model, starve the beast

_starve()
{
  _fifo_dec $deccmd2 tmp
}

# -----------------------------------------------------------------------------
# 3-process model, starve the beast, in a loop

_starve_loop()
{
  _fifo_server							# start server
  sleep 1

  loop=0
  loops=2
  while [ $loop -lt $loops ]
  do
    _say "loop $loop"
    _decdata							# start data sender
    sleep 1
    _deccmd $deccmd3					# start command sender
    let loop=$loop+1
  done

  _decdata								# start data sender
  sleep 1
  _deccmd $deccmd3e						# last fifo client
}

# --------------------------------------------------------------------------------
_all()
{
  _time
  _show
  _fifo
}
# --------------------------------------------------------------------------------

cmd=${1:-all}
shift 

_${cmd} $@
