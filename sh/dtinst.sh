#! /bin/sh

# usage: dtinst.sh dir system
# example: dtinst.sh /usr/erb/prj/dtqw1 qw1
# must be run from the dt main directory
#
# ------------------------------------------------------------
_say()
{
  echo "dtinst.sh: $@" 
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit 1
}
# ------------------------------------------------------------
_run()
{
  if [ -n "$DT_SH" ]
  then
    _say "$@" 1>&2
  fi
  eval "$@"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "rc=$rc doing \"$@\""
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
_dtutil()
{
  if [ ! -x $dtutil ]
  then
    _error "could not find executable $dtutil"
  fi

  $dtutil $@

  if [ $? -ne 0 ] 
  then
    _error "$dtutil failed"
  fi
}
# ------------------------------------------------------------

_argcheck $# 1 99 "$cmd system [make_args]]"

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

dtutil=$OUT/bin/dtutil
make_out=$OUT/make.out
error_out=$OUT/error.out
warning_out=$OUT/warning.out
fail_out=$OUT/fail.out
timing_out=$OUT/timing.out

do_make=1
do_strip=1
do_test=1
do_timing=1

export PWD
_run "$SRC/sh/dtmake.sh $system dtutil"

echo ============================================== > $make_out
_dtutil time dtinst.sh: _date_ _time_ $system      >> $make_out
echo ============================================== > $error_out
_dtutil time dtinst.sh: _date_ _time_ $system      >> $error_out
echo ============================================== > $warning_out
_dtutil time dtinst.sh: _date_ _time_ $system      >> $warning_out
echo ============================================== > $fail_out
_dtutil time dtinst.sh: _date_ _time_ $system      >> $fail_out
echo ============================================== > $timing_out
_dtutil time dtinst.sh: _date_ _time_ $system      >> $timing_out

if [ -n "$do_make" ]
then
  _dtutil time dtinst.sh: _date_ _time_ $system making...
  _run "$SRC/sh/dtmake.sh $system . $@ 2>&1 >>$make_out"
fi

if [ -n "$do_strip" ]
then
  _dtutil time dtinst.sh: _date_ _time_ $system stripping...
  _run "$SRC/sh/dtmake.sh $system . strip 2>&1 >>$make_out"
  _run "rm -f $OUT/bin/*.map"
fi

if [ -n "$do_test" ]
then
  _dtutil time dtinst.sh: _date_ _time_ $system testing...
  _run "$SRC/sh/dttest.sh $OUT"
fi

if [ -n "$do_timing" ]
then
  _dtutil time dtinst.sh: _date_ _time_ $system timing...
  sh $SRC/sh/dttiming.sh $OUT
fi

_dtutil time dtinst.sh: _date_ _time_ $system checking...
_run "$SRC/sh/dtcheck.sh $OUT"

_dtutil time dtinst.sh: _date_ _time_ $system complete >> $make_out
_dtutil time dtinst.sh: _date_ _time_ $system complete >> $error_out
_dtutil time dtinst.sh: _date_ _time_ $system complete >> $warning_out
_dtutil time dtinst.sh: _date_ _time_ $system complete >> $fail_out
_dtutil time dtinst.sh: _date_ _time_ $system complete >> $timing_out


