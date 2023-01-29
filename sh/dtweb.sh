
# ---------------------------------------------------------------------------

say()
{
  echo "dtweb.sh: $@"
}

# ---------------------------------------------------------------------------

run()
{
  if test "$DT_SH" = "1"
  then
    say "$@"
  fi
  eval "$@"
  if [ $? -ne 0 ]
  then
    exit 1
  fi
}
# ---------------------------------------------------------------------------
_nohome()
{
  awk '
    /logo30/ { print "<img align=top src=\"/gifs/dtlogo60.gif\""; next }
    /head3/  { print "<img align=top src=\"/gifs/dthead2.gif\""; next }
             { print $0}
  ' < $web/$1/all.htm > $tmp1
  cp $tmp1 $web/$1/all.htm
  rm $tmp1
}

# ---------------------------------------------------------------------------

set -e

src=/usr/erb/prj/dt
. $src/sh/dtwebenv.sh
 
tmp1=/tmp/dtweb.$$.tmp1
tmp2=/tmp/dtweb.$$.tmp2

say "removing..."
run "rm -rf $web"
run "rm -rf $webparent/texts"
run "rm -rf $webparent/ps3s"
run "rm -rf $webparent/ps6s"
run "rm -rf $webparent/gifs"
run "rm -rf $secure"
run "ls -lt $webparent"
run "ls -lt $webparent/docs"

run "umask 007"
run "mkdir $web"
run "mkdir $webparent/texts"
run "mkdir $webparent/ps3s"
run "mkdir $webparent/ps6s"
run "mkdir $webparent/gifs"
run "mkdir $secure"
run "chmod 710 $secure"
run "mkdir $secure/admin"

# get stuff out of development directory
run "cp $src/doc/html/htaccess   $web/.htaccess"
run "cp $src/doc/html/dtdown.cgi $web/dtdown.cgi"

run "ln -s dthome/ $web/index.html"

say "creating..."

for dir in `cat $src/doc/html/main.list`
do
  say "$web/$dir..."
  mkdir $web/$dir
  grep -v "</*t[ard]" $doc/html/$dir/all.htm >$web/$dir/all.htm
  ln -s all.htm $web/$dir/index.html
done

for dir in `cat $src/doc/html/base.list`
do (
  say "$web/$dir"
  mkdir $web/$dir
  cd $doc/html/$dir
  #                   			get rid of table stuff
  for file in *.htm
  do
    grep -v "</*t[ard]" $file >$web/$dir/$file
  done
  ln -s sect000.htm $web/$dir/index.html
); done

# ---------------------------------------------------------------------------
# replace small header with big header in some documents
ls -lt $web/dthome/all.htm
_nohome dthome
ls -lt $web/dthome/all.htm
_nohome dtfuture
_nohome dtdiscl

# ---------------------------------------------------------------------------
# read the customer database and make secure/passcode/index.html files
run "sh $src/sh/dtwebpc.sh"

# ---------------------------------------------------------------------------
# read the itemlist database and make secure/admin/alg/index.html files
run "sh $src/sh/dtwebalg.sh"

run "cp -r $doc/gif/* $webparent/gifs"

chgrp -R dtack $web

run "chgrp dtackweb $web/dtdown.cgi"
