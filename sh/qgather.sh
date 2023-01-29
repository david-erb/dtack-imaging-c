# gather up all the stuff to distribute a QNX4 SDK release

script=qgather.sh
dtdir=/usr/erb/prj/dt
udir=/usr/erb/devel

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
  _say "$@"
  eval "$@"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$@\""
  fi
}
# ----------------------------------------------------------------------------
# pack up dtack include files from development machine
_dtinc()
{
  _run "rm -f $dtdir/include/*~"
  _run "rm -f $dtdir/fourth/mpegssg/*~"

  _run "cd  $dtdir; tar cf - include/*.[hp] fourth/mpegssg/*.[hp] | \
     freeze -c > /tmp/dtinc.taf"
}
# ----------------------------------------------------------------------------
# pack up dtack libs from development machine
_dtlib()
{
  _run "rm -f ${dtdir}qw1/lib/*"

  _run "cd  $dtdir/fourth; makeqw1 libs"

  _run "cd  $dtdir/source; makeqw1 libs"

  _run "cd  ${dtdir}qw1; tar cf - lib | \
     freeze -c > /tmp/dtlib.taf"
}
# ----------------------------------------------------------------------------
_dtunpack()
{
  _run "cd /dtack; fcat < /tmp/dtinc.taf | tar xf -"
  _run "cd /dtack; fcat < /tmp/dtlib.taf | tar xf -"
}
# ----------------------------------------------------------------------------
_ustate()
{
  _run "cp -vn $dtdir/app/ustate/*.[chp] $udir/apps/ustate"
  _run "cp -vn $dtdir/app/ustate/ustate.use $udir/apps/ustate"
  _run "cp -vn $dtdir/app/ustate/ustate.cfg $udir/apps/ustate/ustate.template.cfg"
}
# ----------------------------------------------------------------------------
_uclient()
{
  _run "cp -vn $dtdir/app/uclient/*.[chp] $udir/apps/uclient"
  _run "cp -vn $dtdir/app/uclient/uclient.use $udir/apps/uclient"
}
# ----------------------------------------------------------------------------
_ustream()
{
  _run "cp -vn $dtdir/app/ustream/*.[chp] $udir/apps/ustream"
  _run "cp -vn $dtdir/app/ustream/ustream.use $udir/apps/ustream"
  _run "cp -vn $dtdir/app/ustream/ustream.sh $udir/apps/ustream"
}
# ----------------------------------------------------------------------------
_ustream_link()
{
  wlink op quiet form qnx flat na ustream op map=ustream.map op priv=3 op c \
    libp /usr/lib: de all l /usr/lib/beta3r.lib f ustream.o \
    l ../../apps/ustream/ustream.lib l ../../apps/ustate/ustate.lib \
    l ../../apps/ustream/dtaudio.lib l /dtack/lib/dtdg.lib \
    l /dtack/lib/dtlow.lib l /dtack/lib/dt1d.lib op offset=40k op st=32k  
}
# ----------------------------------------------------------------------------
_netchat()
{
  _run "cp -vn $dtdir/app/netchat/*.[chp] $udir/apps/netchat"
  _run "cp -vn $dtdir/app/netchat/netchat.use $udir/apps/netchat"
  _run "cp -vn $dtdir/app/netchat/netchat.sh $udir/apps/netchat"
}
# ----------------------------------------------------------------------------
_netchat_link()
{
wlink op quiet form qnx flat na netchat op map=netchat.map op priv=3 op c libp /usr/lib: de all l /usr/lib/beta3r.lib f netchat.o l ../../apps/netchat/netchat.lib l ../../apps/ustate/ustate.lib l ../../nci32/nci.lib l ../../lib32/tokm.lib l /dtack/lib/dtinter.lib l /dtack/lib/dtkeybd.lib l /dtack/lib/dtdg.lib l /dtack/lib/dtlow.lib l /dtack/lib/dt1d.lib op offset=40k op st=32k  
}
# ----------------------------------------------------------------------------

eval _$1
