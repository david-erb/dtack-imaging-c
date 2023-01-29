# script to run simple umplay tests with the Videoplex hardware decoder
# -----------
# scenario 1: test short display
#   umplay_sw.sh short
# success: visual
# -----------
# scenario 2: test long display
#   umplay_sw.sh long
# success: visual
# -----------
# scenario 3: test fifo server (like umplay with mpeg_decode in uniquant)
#   umplay_sw.sh fifo
# runs fifo server in background with some debug to console (not log file)
# runs client to stuff data and a series of stops, starts, and speed changes
# finally client tells server to exit and script checks this
# success: visual
# -----------
# scenario 4: test audio sync after fast forward
#   umplay_sw.sh audiosync
# runs ipc server in background with some debug to console (not log file)
# sends a fast forward command from a script to the server
# after sleeping, sends a normal play command from a script to the server
# success: visual, local
# -----------

script=umplay_vp.sh

mpegdir=/devel/test/mpeg				# where test clips live
testclip1=nci9701a.mpg					# short test clip
testclip2=janice15.mpg					# long test clip
testclip3=count.mpg						# numbers/counting

datadir=/devel/test/umplay				# where data files live
stuff1=stuff1.dat						# simple stuff
stuff2=stuff2.dat						# loop stuff
stuff2e=stuff2e.dat						# loop exit

host=127.0.0.1
port=6999
ipc=rocket,$host:$port

variant=vp

fifodir=/fifo				# where fifos are
cmdfifo=cmdfifo				# names of the fifos
decfifo=decfifo
repfifo=repfifo

showscreen=
showkeyboard=trmios,,ctlkeys

noscreen=dummy
nokeyboard=none

control="-op control -connect $ipc -input"

tmp1=/tmp/umplay_vp.tmp					# a temp file we can write on

#
# -----------------------------------------------------------------------------
_say()
{
  echo "$script: $@" 
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
  _say "$@" 1>&2
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
_short()
{
  _run umplay 							\
    -op file							\
    -input $mpegdir/$testclip1 			\
    -variant $variant					\
    -config /CONFIG/umplay.cfg          \
    -keyboard $showkeyboard -D alimits
}
# -----------------------------------------------------------------------------
_long()
{
  _run umplay 							\
    -op file							\
    -input $mpegdir/$testclip2 			\
    -variant $variant					\
    -keyboard $showkeyboard -D command
}
# -----------------------------------------------------------------------------
_fifo_server()
{
  server="umplay 						\
    -op fifo							\
    -connect $fifodir/$cmdfifo 			\
    -input $fifodir/$decfifo 			\
    -path $fifodir/$repfifo 			\
    -variant $variant					\
    -D command -dbgto stdout -errto stdout"
  _say $server
  $server &
}
# -----------------------------------------------------------------------------
_fifo_client()
{
  _stuff=$1
  _run umplay							\
    -op stuff							\
    -config $datadir/$_stuff			\
    -input $mpegdir/$testclip2 			\
    -connect $fifodir/$cmdfifo 			\
    -output $fifodir/$decfifo 			\
    -path $fifodir/$repfifo
}
# -----------------------------------------------------------------------------
_fifo_prepare()
{
  _run "rm -f $fifodir/$cmdfifo"
  _run "rm -f $fifodir/$decfifo"
  _run "rm -f $fifodir/$repfifo"
  _run "mkfifo $fifodir/$cmdfifo"
  _run "mkfifo $fifodir/$decfifo"
  _run "mkfifo $fifodir/$repfifo"
}
# -----------------------------------------------------------------------------
_fifo()
{
  _fifo_prepare

  _fifo_server							# start fifo server
  sleep 1
  _fifo_client $stuff1					# start fifo client

  _gone fifo							# make sure server is gone
}
# -----------------------------------------------------------------------------
_fifo_loop()
{
  _fifo_prepare

  _fifo_server							# start fifo server
  sleep 1
  loop=0
  loops=10000
  while [ $loop -lt $loops ]
  do
    _say "loop $loop"
    _fifo_client $stuff2				# start fifo client
    let loop=$loop+1
  done

  _fifo_client $stuff2e					# last fifo client

  _gone fifo							# make sure server is gone
}
# -----------------------------------------------------------------------------
_ipc_start()
{
  slay -fQ umplay
  server="umplay 					   	\
    -op server							\
    -connect $ipc						\
    -variant $variant					\
    -keyboard none -D command"
  _say $server
  $server &
}
# -----------------------------------------------------------------------------
_audiosync()
{
  _ipc_start							# start ipc server
  _run sleep 5
  _run umplay $control \"input $mpegdir/$testclip3\"
  _run sleep 3
  _run umplay $control "fforward"
  _run sleep 1
  _run umplay $control "forward"
  _run sleep 6
  _run umplay $control "exit"			# tell server to quit
  _gone server							# make sure server is gone
}
# --------------------------------------------------------------------------------
_all()
{
  _time
  _fifo
  _show
}
# --------------------------------------------------------------------------------

cmd=${1:-all}
shift

_${cmd} $@
