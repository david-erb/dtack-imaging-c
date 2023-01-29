
shell=cmd.sh

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
_copy_class()
{
  _runfg tabchief \
    -connect $ipaddr:5780 \
    -op copyto \
    -in /usr/erb/prj/dt/java/$1 \
    -out c:/tableau/00-31/doc/java/dtack/$1 \
    -D usr1+usr2 \
    -rep 1
}
# -----------------------------------------------------------------------------
_copy_exe()
{
  _runfg tabchief \
    -connect $ipaddr:5780 \
    -op copyto \
    -in /f/tmp/$1 \
    -out $1.new \
    -D usr1+usr2 \
    -rep 1
}
# -----------------------------------------------------------------------------
_copy()
{
  _runfg "tabchief \
    -connect $ipaddr:5780 \
    -op copyto \
    -in \"$1\" \
    -out \"$2\" \
    -D usr1+usr2+ipc \
    -rep 1"
}
# -----------------------------------------------------------------------------
_copyfrom()
{
  _runfg "tabchief \
    -connect $ipaddr:5780 \
    -op copyfrom \
    -in \"$1\" \
    -out \"$2\" \
    -D usr1+usr2 \
    -rep 1"
}
# -----------------------------------------------------------------------------

if [ $# -ne 2 ]
then
  _error "wrong argument count"
fi

bat=$1
if [ ! -r $bat ]
then
  _error "$bat does not exist"
fi

case $2 in
  16) ipaddr=192.9.200.16;;
  gryphon) ipaddr=207.87.44.52;;
  tyrol) ipaddr=208.135.157.102;;
  lena) ipaddr=206.166.88.117;;
  mill) ipaddr=206.166.88.104;;
  newlena) ipaddr=208.135.157.104;;
  badboy) ipaddr=207.49.211.67;;
  cal) ipaddr=207.21.26.34;;
  *) _error "invalid ipaddress $2";;
esac

#  (
#    cd  /f/tmp/lena
#    for file in *
#    do
#      _copy "$file" "c:/tableau/config/$file"
#    done
#  )

#   _copy "/f/tmp/lena/tabstart.arg" "c:/tableau/config/tabstart.arg"
#   _copy "/f/tmp/lena/tabrobot.exe" "c:/tableau/01-04/tabrobot.exe"
#   _copy "/f/tmp/lena/tabgrab.exe" "c:/tableau/01-04/tabgrab.exe"
#   _copy "/f/tmp/lena/camclean.bat" "c:/tableau/01-04/camclean.bat"

#   _copy "tabstart.arg" "c:/tableau/config/tabstart.arg"
#   _copy "tabtiny0.dat" "c:/tableau/config/tabtiny0.arg"
#   _copy "tabtiny1.dat" "c:/tableau/config/tabtiny1.arg"

#  _copy_exe tableau.exe
#  _copy_exe tabfast.exe
#  _copy_exe tabgrab.exe
#  _copy_exe tabrobot.exe
#  _copy_exe tabchief.exe

#  _copy /f/prj/dtdist/scandisk/camclean.bat ./camclean.bat
#  _copy /f/prj/dtdist/scandisk/camclean.pif ./camclean.pif
#  _copy /f/prj/dtdist/scandisk/scandisk.ini ./scandisk.ini
#  _copy /f/prj/dtdist/scandisk/scandisk.exe ./scandisk.exe

#  _copyfrom c:/tableau/01-07/doc/ptz.html /home/pcftp/ptz.html

#  _copy /home/pcftp/ptz.html c:/tableau/01-07/doc/ptz.html

  _runfg tabchief \
    -connect $ipaddr:5780 \
    -op copyto \
    -in $bat \
    -out c:/windows/temp/cmd.bat \
    -D usr1+usr2 \
    -rep 1

  _runfg tabchief \
    -connect $ipaddr:5780 \
    -op run \
    -in c:/windows/temp/cmd.bat \
    -D usr1+usr2 \
    -rep 1 \
    | grep "<p><b>"

  _runfg "sleep 5"

  _runfg tabchief \
    -connect $ipaddr:5780 \
    -op copyfrom \
    -in /tmp/cmd3.out \
    -out cmd3.out \
    -D usr1+usr2+trace \
    -rep 1 

  cat cmd3.out





