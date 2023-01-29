#! /bin/sh
# scan html files in directory, changing figures to gifs and thumbnails
# ---------------------------------------------------------------------------
run()
{
  if test "$DT_SH" = "1"
  then
    echo "dthvalid.sh: $@"
  fi
  eval "$@"
  if test ! $?
  then
    echo "dthvalid.sh: exiting"
    exit 1
  fi
}
# ---------------------------------------------------------------------------
# run validation tool on one html
#   <img align=top src="x/y.pgm.gz">
validate()
{
  html-check $1
}
# ---------------------------------------------------------------------------

if test $# -ne 1
then
  echo "dthvalid.sh: usage html_dir"
  exit 1
fi

docdir=/usr/erb/prj/dtdoc/doc
if [ ! -d $docdir/gif ]
then
  mkdir $docdir/gif
fi

cd $1
for file in *.htm
do
  validate $file
done
