#! /bin/sh

# Dtack make script for BSD-like Unix systems (Linux, FreeBSD, Solaris, Sunos, IRIX)
# usage: dtmake.sh OSNAME [ SUBDIR [ TARGET ] ]
# where:
#   OSNAME is the name of both the .set file and the suffix of the object tree
#   SUBDIR is subdir relative to this to compile in, defaults to '.'
#   TARGET target within makefile in subdir to make, defaults to ''
# This script must be executed while in the top of the source tree.
# examples:
# make everything for sunos:
#   sh/dtmake.sh sunos 
# make subdir source for sunos:
#   sh/dtmake.sh sunos source
# make subdir source, only the libraries for sunos:
#   sh/dtmake.sh sunos source libs

# environment variables:
# SRC=top of the source tree, defaults to .
# OUT=top of the object tree, defaults to $pwd.OSNAME
# DT_SET=where the .set files are stored, by default searches for 'dtack'
#   in $pwd and appends /config

# ------------------------------------------------------------
_say()
{
  echo "dtmake.sh: $@" 
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit 1
}
# ------------------------------------------------------------
_need_dir()
{
  if [ ! -d "$1" ]
  then
    _error "Sorry, the $2 directory \"$1\" cannot be found."
  fi
}
# ------------------------------------------------------------
_need_file()
{
  if [ ! -r "$1" ]
  then
    _error "Sorry, the $2 file \"$1\" cannot be found."
  fi
}
# ------------------------------------------------------------
_run()
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
_make()
{ 
  _need_file "$DT_SET/$system.set" "make control file"
  _need_file "$DT_SET/bsd1.set" "make control file"
  _run "make \
-f $DT_SET/bsd1.set \
-f $DT_SET/$system.set \
-f bsd1.mak \
SRC=$SRC \
OUT=$OUT \
MK_TARGET=$system \
$make_args 2>&1"
}
# ------------------------------------------------------------

_argcheck $# 1 99 "$cmd system [subdir [make_args]]"

system=$1
shift

#					......................................
if [ -z "$SRC" ]			# SRC not set?
then
  SRC=`pwd`				# presume current directory
fi
export SRC

if [ -z "$OUT" ]			# OUT not set?
then
  OUT=`pwd`.$system			# presume current directory.system
fi
export OUT

#					......................................
if [ $# -ge 1 ]				# second arg available?
then
  subdir="$SRC/$1"			# just making a subdirectory
  shift
else					# second arg not available?
  subdir="."				# make all subdirectories
fi

#					......................................
export DT_SET
if [ -z "$DT_SET" ]			# DT_SET not set?
then
  pwd=`pwd`
  sub=`echo $pwd|sed 's,^.*dtack/,,'`   # find dtack subdirectory
  dtack=`echo $pwd| sed "s,$sub\$,,"`
  if [ -d "$dtack/config" ]		# but dtack/config here?
  then
    DT_SET="$dtack/config"		# get .set files from dtack/config
  else
    DT_SET="."				# get .set files from current dir
  fi
fi

_need_dir "$DT_SET" "yam control"

#					......................................

make_args="$@"
cd $subdir

_make

exit 0



