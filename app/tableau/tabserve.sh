#!/bin/sh

script="tabserve.sh"

# caller is responsible for setting PATH and LD_LIBRARY_PATH

# -----------------------------------------------------------------------------
# Theory of Operation
# The Tableau Server-side is a forking server.
# This means that a "momma" process listens permanently on a known port.
# When a client connects, the momma forks a child to handle the conversation.
# When the conversation is over, the child exits.
# 
# A client to this server is a capture process.
# The purpose of this server is to receive an image stream from a capture process.
# 
# Installation
# Run this script from /etc/rc.local or equivalent.
# Before running it, set up the PATH and LD_LIBRARY_PATH.
# Edit the "imagepath" to point to the directory where image subdirectories are.
# Edit the "log" to give the name of a log file.

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
_start()
{
  imagepath=./jpeg			# where image subdirectories are
  log=stdout				# where log files are written

  _cmd="tableau \
    -conn rocket,0:5710 \
    -path $imagepath \
    -screen roll \
    -psi 50000 \
    -variant passthrough \
    -dz 10.0 \
    -D usr1+usr2+ipc \
    -errto $log \
    -dbgto $log \
    -sayto $log" 

  _say $_cmd
  eval $_cmd &
}
# -----------------------------------------------------------------------------

_start

exit
