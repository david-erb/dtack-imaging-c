
# ---------------------------------------------------------------------------

run()
{
  if test "$DT_SH" = "1"
  then
    echo "dtweb.sh: $@"
  fi
  eval "$@"
  if test ! $?
  then
    exit 1
  fi
}
# ---------------------------------------------------------------------------
set -e

src=/usr/erb/prj/dt
. $src/sh/dtwebenv.sh
 
# gif and xbm are really peer directories to web (html)
run "cp -R $doc/gif $web/../gif"
run "cp -R $doc/xbm $web/../xbm"
run "cp $src/doc/html/site1.map $web/../gif/dtsite"
run "cp $src/doc/html/site1.gif $web/../gif/dtsite"
