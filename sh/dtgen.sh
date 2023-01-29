#
# ------------------------------------------------------------
_say()
{
  echo "dtgen.sh: $@" 
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
  if [ $? -ne 0 ]
  then
    _error "doing \"$@\""
  fi
}
# ------------------------------------------------------------
_needdir()
{
  if [ ! -d $1 ]
  then
    _error "directory $1 does not exist"
  fi
}
# ------------------------------------------------------------
_needfile()
{
  if [ ! -r $1 ]
  then
    _error "file $1 does not exist"
  fi
}
# ------------------------------------------------------------

src=/usr/erb/prj/dt

if [ $# -ne 2 ]
then
  _error "expect command line like dtgen.sh /usr/erb/prj/dt00-01 ."
fi
root=$1
dir=$2

_needdir $root.template/$dir
_needfile $root.template/$dir/manifest.txt

if [ "$dir" = "." ]
then
  _say "removing old stuff"
  if [ -d "$root" ]
  then
    _run "rm -rf $root/*" 
  else
    _run "mkdir $root"
  fi
else
  _run "rm -rf $root/$dir"
  _run "mkdir $root/$dir"
fi

subdirs=`cut -f1 <$root.template/$dir/manifest.txt | grep "/$" | sed 's./$..'`
files=`cut -f1 <$root.template/$dir/manifest.txt | grep -v "/$"`

cp -p $root.template/$dir/manifest.txt $root/$dir

for subdir in $subdirs
do
  _run "$src/sh/dtgen.sh $root $dir/$subdir"
done

_say "copying `echo $files|wc -w` files to $root/$dir"

for file in $files
do
  _run "cp -p $src/$dir/$file $root/$dir"
done

if [ "$dir" = "." ]
then
  _say "setting permissions"
  _run "chmod -R a+rw $root"
  _run "chmod -R a+x $root/sh"
  _run "find $root -type d -exec chmod a+x {} \;"
fi

exit


