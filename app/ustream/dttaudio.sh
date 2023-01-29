# script to test audio device recording and playback
# -----------
# dttaudio.sh I
#   play pre-recorded clip
# -----------

script=dttaudio.sh

itype=sbqnx,/dev/dsp					# audio itype

clip1=/devel/test/uaudio/houston.pcm	# pre-recorded clip

tmpdir=/tmp								# where to write test files
tmp1=$tmp/dttaudio1.tmp					# tmp file
size=30000								# size of tmp file

export DT_DIALOG=none

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
_runq()
{
  eval "$@"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$@\""
  fi
}
# -----------------------------------------------------------------------------
_run()
{
  _say "$@" 1>&2
  _runq "$@"
}
# -----------------------------------------------------------------------------
# play pre-recorded clip

_play()
{
  _say "playing at full volume"
  _run "dttaudio -op play -input $clip1 $itype,100"
  _say "playing at half volume"
  _run "dttaudio -op play -input $clip1 $itype,50"
}
# -----------------------------------------------------------------------------

_record()
{
  _say "recording at full volume"
  _run "dttaudio -op record -output $tmp1 -n $size $itype,100"
  _say "playing at full volume"
  _run "dttaudio -op play -input $tmp1 $itype,100"

  _say "recording at half volume"
  _run "dttaudio -op record -output $tmp1 -n $size $itype,50"
  _say "playing at full volume"
  _run "dttaudio -op play -input $tmp1 $itype,100"

  rm -f $tmp1
}

# --------------------------------------------------------------------------------
_all()
{
  _play
  _record
}
# --------------------------------------------------------------------------------

cmd=${1:-all}

_${cmd}
