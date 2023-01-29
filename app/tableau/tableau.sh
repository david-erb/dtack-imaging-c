# script to run automated netchat tests

script=tableau.sh

exe=tableau
ipaddr=`hostname`
moist=www.dtack.com
digiweb=206.161.225.2       #columbia.digiweb.com
topsolution=206.245.142.105 #ipromote.topsolution.com
server_port=5711
ident_port=5714
path=/img/tableau
stream=test1

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
# -----------------------------------------------------------------------------
_runfg()
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
_runbg()
{
  _sayq "$@"
  $@ &
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "($rc) doing \"$@\""
  fi
}
# -----------------------------------------------------------------------------
_gen_daemon()
{
  _argcheck $# 0 0 "$cmd no arguments"

  rm -f /tmp/tableau.log

  _runfg "$exe \
     -connect rocket,$ipaddr:5713 \
     -input gen,count,320,240 \
     -color luminance \
     -config /tableau/config \
     -psi 50000 \
     -variant jpeg \
     -dz 5.0 \
     -t 100 \
     -D usr1+usr2+ipc+command \
     -errto stdout \
     -dbgto stdout \
     -sayto stdout"
}
# -----------------------------------------------------------------------------
_gen_client()
{
  _argcheck $# 1 1 "$cmd host"

  _host=$1

  _runfg "$exe \
     -input gen,count,320,240 \
     -screen hws,x \
     -psi 50000 \
     -variant jpeg \
     -D usr1+usr2+ipc+trace+command+object \
     -n 5 \
     -errto stdout \
     -dbgto stdout \
     -sayto stdout"
}
# -----------------------------------------------------------------------------
_tabgrab_display()
{
  _argcheck $# 1 1 "$cmd host"

  _host=$1

  _runfg "$exe \
     -input file,rocket,$_host:5720 \
     -screen hws,x \
     -D usr1+usr2 \
     -variant passthrough \
     -errto stdout \
     -dbgto stdout \
     -sayto stdout"
}
# -----------------------------------------------------------------------------
_meteor_roll()
{
  _argcheck $# 0 0 "$cmd"

  _host=$1

  _runfg "$exe \
     -config /tableau/config \
     -input meteor,/dev/meteor0,320,240 \
     -screen roll,test1 \
     -path /dd/2/img/tableau \
     -D usr1+usr2 \
     -n 5 \
     -dz 5 \
     -errto stdout \
     -dbgto stdout \
     -sayto stdout"
}
# -----------------------------------------------------------------------------
_server_http()
{
  _argcheck $# 0 0 "$cmd no arguments"

  rm -f /tmp/tableau.log

  _runfg "$exe \
     -connect rocket,$ipaddr:0 \
     -path $path \
     -psi 50000 \
     -variant passthrough \
     -dz 5.0 \
     -D usr1+usr2+ipc+trace \
     -errto /tmp/tableau.log \
     -dbgto /tmp/tableau.log \
     -sayto /tmp/tableau.log"
}
# -----------------------------------------------------------------------------
_javalike()
{
  _argcheck $# 0 0 "$cmd no arguments"

#     -screen roll,test1 
  _runfg "$exe \
     -input rocket,208.135.157.104:5712 \
     -screen hws,x \
     -path /usr/dtack/root/web/docs/jpeg \
     -variant uncompressed \
     -dx 2 -dy 2 \
     -D usr1+usr2"
}
# -----------------------------------------------------------------------------
# act as exclusive server ready to answer HTTP requests
_exclusive_http()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
     -connect rocket,$ipaddr:5712^ \
     -input gen,sweep,320,240 \
     -psi 1000 \
     -variant jpeg \
     -config /tableau/config \
     -dz 5.0 \
     -D usr1+usr2+ipc+config+command+trace"
}
# -----------------------------------------------------------------------------
# act as exclusive server which takes uses ipc to transfer
_exclusive_ipc()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
     -connect rocket,$ipaddr:5712! \
     -input gen,sweep,320,240 \
     -psi 1000 \
     -variant jpeg \
     -config /tableau/config \
     -dz 5.0 \
     -D usr1+usr2+ipc+config+command+trace"
}
# -----------------------------------------------------------------------------
# act as exclusive server to store to roll file
_exclusive_roll()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
     -connect rocket,$ipaddr:5710! \
     -screen roll,test1 \
     -path /dd/2/img/tableau \
     -variant passthrough \
     -dz 5.0 \
     -D usr1+usr2"
}
# -----------------------------------------------------------------------------
_capture_server()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
    -connect rocket,$ipaddr:$server_port \
    -path $path \
    -screen http://$topsolution:80/nph-tabserve.cgi?ident=cap09 \
    -psi 50000 \
    -variant passthrough \
    -dz 5.0 \
    -D usr1+usr2 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout" 
}
# -----------------------------------------------------------------------------
_capture_roll()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
    -connect rocket,$ipaddr:$server_port \
    -path $path \
    -screen roll,$stream \
    -psi 50000 \
    -variant passthrough \
    -dz 5.0 \
    -D usr1+usr2 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout" 
}
# -----------------------------------------------------------------------------
_capture_http()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "cp /usr/erb/prj/dtfg1/bin/$exe /usr/dtack/root/web/tab00-27/$exe"

  _runfg "$exe \
    -input dir,/img/tableau/gary2.raw/%03d,256,256,0 \
    -config /tableau/config \
    -screen http://$ipaddr:80/nph-tabserve.cgi?ident=$stream \
    -D usr1+usr2+reply \
    -op \"stamp,test %d %b %y %H:%M:%S\" \
    -dz 5 \
    -n 10 \
    -t 25 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------
_gen_ipc()
{
  _argcheck $# 2 2 "$cmd host port"

  _host=$1
  _port=$2

  _runfg "$exe \
    -input gen,clock,320,240 \
    -config /tableau/config \
    -screen rocket,$_host:$_port+$stream \
    -color luminance \
    -D usr1+usr2+ipc \
    -op \"stamp,test %d %b %y %H:%M:%S\" \
    -n 10 \
    -t 25 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------
_gen_clock()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
    -input gen,clock,320,240 \
    -config /tableau/config \
    -screen roll,$stream \
    -color luminance \
    -path $path \
    -D usr1+usr2 \
    -op \"stamp,test %d %b %y %H:%M:%S\" \
    -dz 10 \
    -dt 500 \
    -t 25 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------
_gen_count()
{
  _argcheck $# 0 0 "$cmd no arguments"

  _runfg "$exe \
    -input gen,count,320,240 \
    -config /tableau/config \
    -screen roll,$stream \
    -path $path \
    -D usr1+usr2 \
    -op \"stamp,test %d %b %y %H:%M:%S\" \
    -dz 10 \
    -dt 500 \
    -t 25 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------
_gen_http()
{
  _argcheck $# 2 3 "$cmd host stream [localport]"

  _host=$1
  _stream=$2
  _localport=${2:-5781}


  _runfg "$exe \
    -input gen,sweep,200,200 \
    -color luminance \
    -screen http://$_host:80/cgi-bin/nph-perserve.cgi?ident=$_stream\&dz=10\&D=usr1+usr2+ipc \
    -config /tableau/config \
    -D usr1+usr2+reply+trace+ipc \
    -op \"stamp,test %d %b %y %H:%M:%S\" \
    -variant jpeg \
    -psi 50000 \
    -dz 5 \
    -t 25 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------
_gen_ftp()
{
  _argcheck $# 0 0 "$cmd"

  _runfg "$exe \
    -input gen,sweep,200,200 \
    -config /tableau/config \
    -screen ftp,/webcam/unit1/%s.%08ld.jpg \
    -D usr1+usr2 \
    -op \"stamp,test %d %b %y %H:%M:%S\" \
    -dz 5 \
    -t 25 \
    -dt 300 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------
_thumbnail()
{
  _argcheck $# 0 0 "$cmd"

  if [ ! -d /img/tableau/test1/thumbnail ]
  then
    mkdir /img/tableau/test1/thumbnail
  fi
  rm -f /img/tableau/test1/thumbnail/*
  _runfg "$exe \
    -path /img/tableau \
    -input ser,test1/cam0.pat \
    -screen roll,test1/thumbnail \
    -D usr2 \
    -op resize4-16 \
    -variant jpeg \
    -color rgb888 \
    -n 1 \
    -t 25 \
    -dt 300 \
    -errto stdout \
    -dbgto stdout \
    -sayto stdout"
}
# -----------------------------------------------------------------------------

_argcheck $# 1 99 "$cmd subcommand"

cmd=${1:-all}

shift 

_${cmd} $@


#ifdef __USAGE
#/*..........................................................................
#| NAME
#| netchat.sh - netchat process starting and testing
#|
#| USAGE
#| usage: netchat.sh op [args]
#| op	operation (see below)
#| args	depend on operation
#| 
#| This script starts netchat from the sysinit or the custom menu.
#| It is also used for testing the netchat system.
#|
#| The operations and their arguments are:
#| node_init				start shoveler from sysinit 
#| node_start				start netchat dialog from custom menu 
#| node_restart				restart shoveler and dialog from custom menu 
#|
#| auto1 AUDxx				automated test a single dialog
#| auto2 AUDxx AUDyy		automated test two dialogs
#| auto3 AUDxx AUDyy AUDzz	automated test three dialogs
#|
#| qc2 AUDxx AUDyy			semi-automated test two dialogs
#| qc3 AUDxx AUDyy AUDzz	semi-automated test three dialogs
#| 
#| END
#*..........................................................................*/
#endif
