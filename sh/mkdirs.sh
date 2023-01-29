#! /bin/sh

# create subdirectory tree

script="mkdirs.sh"

# examples:
# sh/mkdirs.sh /usr/local/dtack/iqsign.solaris

# ------------------------------------------------------------
_say()
{
  echo "mkdirs.sh: $@" 
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit 1
}
# ------------------------------------------------------------
_runfg()
{
  if [ -n "$DT_SH" ]
  then
    _say "$@" 1>&2
  fi
  eval "$@"
  if [ $? -ne 0 ]
  then
    _error "doing \"$@\""
  fi
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
# ------------------------------------------------------------
_mkdir()
{
  if [ -d $1 ]
  then
    _say "directory $1 already exists, ok"
  else
    _say "making directory $1"
    _runfg "mkdir $1"
  fi
}
# ------------------------------------------------------------

_argcheck $# 1 1 "$cmd newtree"

top=$1

_mkdir $top

_mkdir $top/dtutil

_mkdir $top/fourth
_mkdir $top/fourth/btpcv3
_mkdir $top/fourth/mpegssg
_mkdir $top/fourth/xv310a
_mkdir $top/fourth/zlib
_mkdir $top/fourth/jpeg6a
_mkdir $top/fourth/emsif
_mkdir $top/fourth/xmsif
_mkdir $top/fourth/svgacc
_mkdir $top/fourth/fastg

_mkdir $top/source
_mkdir $top/source/1d
_mkdir $top/source/low
_mkdir $top/source/dg
_mkdir $top/source/bigmem
_mkdir $top/source/keybd
_mkdir $top/source/inter
_mkdir $top/source/fb
_mkdir $top/source/mpeg
_mkdir $top/source/color
_mkdir $top/source/screen
_mkdir $top/source/win
_mkdir $top/source/high
_mkdir $top/source/xchg

_mkdir $top/test
_mkdir $top/test/lib
_mkdir $top/test/high
_mkdir $top/test/inter
_mkdir $top/test/bigmem
_mkdir $top/test/low
_mkdir $top/test/ftp
_mkdir $top/test/1d
_mkdir $top/test/xchg
_mkdir $top/test/view
_mkdir $top/test/win
_mkdir $top/test/fb
_mkdir $top/test/screen
_mkdir $top/test/xms

_mkdir $top/tool
_mkdir $top/server

_mkdir $top/lib
_mkdir $top/libso
_mkdir $top/bin
_mkdir $top/sh
_mkdir $top/data
_mkdir $top/tmp
