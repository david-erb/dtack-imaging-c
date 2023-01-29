#
# ------------------------------------------------------------
_say()
{
  echo "dtdist.sh: $@" 
}
# ------------------------------------------------------------
_run()
{
  _say "$@"
  eval "$@"
  if [ $? -ne 0 ]
  then
    _say "ERROR doing $@"
    exit 1
  fi
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit
}

os=$1
bin=/usr/erb/prj/dt.$1/bin
version=$2
product=$3
dist=/usr/erb/prj/dtdist/$os/$version


if [ $# -ne 3 ]
then
  _error "usage: os version product (e.g. fg1 01-05 tableau)"
fi

if [ ! -d $bin ]
then
  _error "directory $bin does not exist"
fi

_run "rm -rf $dist"
_run "mkdir $dist"

_run "cd /usr/erb/prj/ijg; make$os /usr/erb/prj/ijg ACT=libs"
_run "cd /usr/erb/prj/xv; make$os /usr/erb/prj/xv ACT=libs"
_run "cd /usr/erb/prj/dt/source; make$os /usr/erb/prj/dt ACT=libs"
_run "cd /usr/erb/prj/dt/source/fb; make$os /usr/erb/prj/dt ACT=libs"
_run "cd /usr/erb/prj/dt/app/reverb; make$os /usr/erb/prj/dt ACT=libs"

if [ "$product" = "tableau" ]
then
  exes="tabchief tableau"
elif [ "$product" = "tabptz" ]
then
  exes="uclient tabrobot tabptz"
elif [ "$product" = "percom" ]
then
  exes="wrangler tabdrone tabdrona"
fi

for exe in $exes
do
  if [ -d /usr/erb/prj/dt/app/$exe ]
  then
    _run "cd /usr/erb/prj/dt/app/$exe; make$os /usr/erb/prj/dt ACT=static"
  fi
  _run "cp $bin/$exe.static $dist/$exe" 
  _run "strip $dist/$exe"
done

_run "cp /usr/erb/prj/dt/app/tableau/unix.readme $dist/$os.readme"

tarfile=/usr/erb/prj/dtdist/$product.$version.$os.tgz
_run "cd $dist/..; tar cf - $version | gzip -c > $tarfile"

_run "ls -l $tarfile"