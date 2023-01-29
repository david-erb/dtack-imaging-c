# script to run simple umplay tests with the software decoder
# -----------
# umplay.sh show
# test software display
# displays a short clip to the default display at 3 speeds: I, P and B
# comparing this to scenario 2 will indicate the display overhead
# success: visual
# -----------
# umplay.sh time
# test software decode time
# times decoding with no display of a short clip at 3 speeds: I, P and B
# success: times printed
# -----------
# umplay.sh stuff
# test fifo server (like umplay with mpeg_decode in uniquant)
# runs fifo server in background with some debug to console (not log file)
# runs client to stuff data and a series of stops, starts, and speed changes
# finally client tells server to exit and script checks this
# success: hand waving at various speeds
# -----------
# umplay.sh normal
# test fifo server with pinging NOPs
# runs fifo server in background with some debug to console (not log file)
# runs one client to stuff data
# runs another client to stuff a series of stops, starts, and speed changes
# finally client tells server to exit and script checks this
# success: about 6 seconds of play
# -----------
# umplay.sh starve
# same as normal, except different script
# test that fifo server can handle data starvation
# success: noticeable pause at 43 and 46
# -----------
# umplay.sh starve_loop
# same as normal, except only one fifo server is run but many
# client pairs come and go
# success: plays up to 56 or 00 with various pauses then restarts at 42
# -----------
# umplay.sh starve_command
# same as normal, except different script
# test that fifo server is not confused by multiple commands coming
# while data is starved out
# success: noticeable pause at 43 and 44
# -----------
# umplay.sh step
# same as normal, except different script
# test that single step really goes only one frame
# success: 30 single frames give exactly 1 second change
# -----------

script=umplay.sh

if [ -z "$NODE" ]
then
  ipc=rocket,0:7081						# server ipc
else
  ipc=qnx,/nci/umplay$NODE				# server ipc
fi

ipc_decdata=rocket,0:7089				# decdata test ipc

mpegdir=/devel/test/mpeg				# where test clips live
testclip1=nci9701a.mpg					# short test clip
testclip2=janice15.mpg					# long test clip
testclip2=big.mpg					# really long test clip

fifodir=/fifo							# where fifos are
cmdfifo=cmdfifo							# names of the fifos
decfifo=decfifo
repfifo=repfifo

datadir=/devel/test/umplay				# where data files live

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
_sw_time()
{
  _run time								\
  umplay 								\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant ssg,raw,I,$noscreen 		\
    -keyboard $nokeyboard -D complex
  _run time								\
  umplay 								\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant ssg,raw,P,$noscreen 		\
    -keyboard $nokeyboard -D complex
  _run time								\
  umplay 								\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant ssg,raw,B,$noscreen		\
    -keyboard $nokeyboard -D complex
}
# -----------------------------------------------------------------------------
_sw_show()
{
  _run time								\
  umplay 								\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant ssg,raw,I,$showscreen 		\
    -keyboard $showkeyboard -D complex
  umplay 								\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant ssg,raw,P,$showscreen 		\
    -keyboard $showkeyboard -D complex
  _run time								\
  umplay 								\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant ssg,raw,B,$showscreen 		\
    -keyboard $showkeyboard -D complex
}
# -----------------------------------------------------------------------------
_short()
{
  _run umplay 							\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant $variant					\
    -config /CONFIG/umplay.cfg          \
    -keyboard $showkeyboard
}
# -----------------------------------------------------------------------------
_long()
{
  _run umplay 							\
    -op file							\
    -input $mpegdir/$testclip2 			\
    -variant $variant					\
    -keyboard $showkeyboard
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
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo \
    -variant $variant					\
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
    -input $mpegdir/$testclip1 			\
    -config $datadir/$_stuff			\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo
}
# -----------------------------------------------------------------------------
# run the fifo server in the background, then the stuff client
# the stuff client sends both data and commands on the fifos
# when client is done, check to make sure server has exited

_stuff1()
{
  _fifo_server							# start server
  sleep 1
  _fifo_client varied.stuff
  _gone fifo							# make sure server is gone
}
# -----------------------------------------------------------------------------
# simple normal play

_stuff2()
{
  _fifo_server							# start server
  sleep 1
  _fifo_client normal.stuff
  _gone fifo							# make sure server is gone
}
# -----------------------------------------------------------------------------
# like stuff2, except start out in ff mode 

_stuff3()
{
  _fifo_server							# start server
  sleep 1
  _fifo_client fast.stuff
  _gone fifo							# make sure server is gone
}
# -----------------------------------------------------------------------------
# like stuff2, except start out in so mode 

_stuff4()
{
  _fifo_server							# start server
  sleep 1
  _fifo_client slow.stuff
  _gone fifo							# make sure server is gone
}
# -----------------------------------------------------------------------------
# all the stuff tests

_stuff()
{
  _stuff1
  _stuff2
  _stuff3
  _stuff4
}

# -----------------------------------------------------------------------------
_deccmd()
{
  _deccmd=$1
  _run umplay 							\
    -op deccmd							\
    -connect $ipc_decdata				\
    -config $datadir/$_deccmd			\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo
}

# -----------------------------------------------------------------------------
_decdata()
{
  decdata="umplay						\
    -op decdata							\
    -connect $ipc_decdata				\
    -input $mpegdir/$testclip2 			\
    -path $fifodir/$decfifo+$fifodir/$cmdfifo+$fifodir/$repfifo"
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
  _fifo_dec normal.deccmd
}

# -----------------------------------------------------------------------------
# 3-process model, starve the beast

_starve()
{
  _fifo_dec starve.deccmd
}

# -----------------------------------------------------------------------------
# 3-process model, starve the beast, in a loop

_starve_loop()
{
  _fifo_server							# start server
  sleep 1

  loop=0
  loops=10000
  while [ $loop -lt $loops ]
  do
    _say "loop $loop"
    _decdata							# start data sender
    sleep 1
    _deccmd starve_loop.deccmd			# start command sender
    let loop=$loop+1
  done

  _decdata								# start data sender
  sleep 1
  _deccmd exit.deccmd					# last fifo client
}

# -----------------------------------------------------------------------------
# 3-process model, give commands while starving

_starve_command()
{
  _fifo_dec starve_command.deccmd
}

# -----------------------------------------------------------------------------
# 3-process model, give commands while starving

_step()
{
  _fifo_dec step.deccmd
}

# --------------------------------------------------------------------------------
_all()
{
  _short
  _long
  _stuff
}
# --------------------------------------------------------------------------------

variant=$1
shift
cmd=${1:-all}
shift 

_${cmd} $@
