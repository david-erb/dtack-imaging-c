
# ---------------------------------------------------------------------------
# read the customer database and make secure/passcode/index.html files

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
_makeawk()
{
(
echo '
BEGIN { 
  FS = "\t"; 
  id = ++n
  passcode = ++n
  modules = ++n
}
# ==============================
function sayn(string)
{
  printf("%s", string) >> "/dev/stderr"
}
# ==============================
function say(string)
{
  printf("dtwebpc.sh: \"%s\"\n", string) >> "/dev/stderr"
}
# ==============================
function run(command)
{
  if (DT_SH == 1)
    say(command)
  system(command)
}
# ==============================
function error(string)
{
  printf("%s\n", string) >> "/dev/stderr"
  exit
}
# ==============================
FILENAME == "-" {
  split($0, f)
  nmodules = split(f[modules], module, ",")
  passdir = secure "/" f[passcode]
  run("rm -rf " passdir)
  run("mkdir " passdir)

  index_html = passdir "/index.html"
  if (f[passcode] == "free")
    run("sed s/PASSCODE/Free/ <"src"/doc/html/webcust1.htm >"index_html)
  else
    run("sed s/PASSCODE/"f[passcode]"/ <"src"/doc/html/webcust1.htm >"index_html)

  say("passcode "f[passcode]"...")
  for (m=1; m<=nmodules; m++)
  {
    sayn("  "module[m])
    one(module[m], module[m])
    run("ln -s ../"admin"/" module[m] " " passdir"/"module[m])
  }
  sayn("\n")
  run("cat " src "/doc/html/webcust2.htm >> " index_html)
}
'

cat $src/sh/dtweb1.awk

cat $src/sh/dtpage.awk
) > $awk
}

# ---------------------------------------------------------------------------
set -e

src=/usr/erb/prj/dt
. $src/sh/dtwebenv.sh

awk=/tmp/dtwebpc.$$.awk
data=$src/doc/products.dat
dtpage=/tmp/dtwebpc.$$.dtpage

sh $src/sh/dtdocm4.sh $src
cat $src/doc/doclist.m4 $data | \
sh $src/sh/dtinclude.sh > $dtpage

_makeawk

cat $src/doc/customer.dat | \
awk \
    -v src=$src \
    -v doc=$doc \
    -v web=$web \
    -v webparent=$webparent \
    -v webroot=$webroot \
    -v secure=$secure \
    -v admin=$admin \
    -v dtpage=$dtpage \
    -v DT_SH=$DT_SH \
    -f /tmp/dtwebpc.$$.awk $dtpage -

