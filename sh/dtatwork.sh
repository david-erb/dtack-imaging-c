#! /bin/sh
# scan html files in directory, setting icon for links
# ---------------------------------------------------------------------------
run()
{
  if test "$DT_SH" = "1"
  then
    echo "dtatwork.sh: $@"
  fi
  eval "$@"
  if test ! $?
  then
    echo "dtatwork.sh: exiting"
    exit 1
  fi
}
# ---------------------------------------------------------------------------
# scan for lines of form: 
#   <img src="dtfollow.gif"> <a href="link">text</a>  or
#   <img src="dtatwork.gif"> <a href="link">text</a> 
atwork()
{
  echo "dtatwork.sh: $file"
  rm -f /tmp/dtatwork.log
  awk -v relative="$relative" '
    BEGIN { FS = "\"" 
      logfile = "/tmp/dtatwork.log"
    }
    {print $0}
    $1 == "<a href=" { 
      if (index($2, ":") == 0 &&
          index($2, "#") == 0 &&
	  system("test -r "$2))
      {
        print "<img src=\""relative"atwork.gif\">"
        printf("  link %s not there\n", $2) >> logfile
      }
    }

  ' < $file > /tmp/$file
  mv /tmp/$file $file
  if [ -r /tmp/dtatwork.log ]
  then
    cat /tmp/dtatwork.log
  fi
}
# ---------------------------------------------------------------------------

if test $# -ne 1 -a $# -ne 2
then
  echo "dtatwork.sh: usage html_dir"
  exit 1
fi

relative=${2:-"./"}

cd $1
for file in *.htm
do
  atwork $1
done
