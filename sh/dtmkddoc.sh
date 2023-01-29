# make directory structure

do_mkdir()
{
  if [ -d $1 ]
  then
    echo "$id: directory $1 already exists"
  else
    echo "$id: making directory $1"
    mkdir $1
  fi
}

if [ $# -ne 1 ]
then
  echo "dtmkdirs.sh: wrong argument count"
  exit 1
fi

top=$1

do_mkdir $top

do_mkdir $top/doc

do_mkdir $top/doc/text
do_mkdir $top/doc/dvi
do_mkdir $top/doc/dvl
do_mkdir $top/doc/ps3
do_mkdir $top/doc/ps6
do_mkdir $top/doc/lj3
do_mkdir $top/doc/lj4
do_mkdir $top/doc/html
do_mkdir $top/doc/info
