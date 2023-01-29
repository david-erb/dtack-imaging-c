# ---------------------------------------------------------------------------
_makeawk()
{
(
echo '
BEGIN { 
  FS = "\t"; 
  partno = ++n
  title = ++n
  deliverable = ++n
  price = ++n
  sales_lit = ++n
  tech_reference = ++n
  tech_summary = ++n
  down_load = ++n
  id = ++n
}
function say(string)
{
  printf("dtwebalg.sh: \"%s\"\n", string) > "/dev/stderr"
}
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
function merge(id, field)
{
  if (fields[id field] == "" ||
      fields[id field] == "_")
    fields[id field] = f[field]
}
# ==============================
FILENAME == "-" {
  split($0, f)
  if (have[f[id]] == 0)
    ids[ndocs++] = f[id]
  have[f[id]] = 1
  merge(f[id], title)
  merge(f[id], sales_lit)
  merge(f[id], tech_reference)
  merge(f[id], tech_summary)
  merge(f[id], down_load)
}
END {
  for (i=0; i<ndocs; i++)
    if (form == "html")
      end_html(ids[i])
}
# ==============================
function end_html(doc_id)
{
  dir = secure "/" admin "/" doc_id
  index_html = dir "/index.html"

  say(doc_id"...")
  run("rm -rf " dir)
  run("mkdir " dir)
  run("cp " doc "/html/" doc_id "/* " dir)
  printf("this will be source code for %s in tar.gz format\n", 
    doc_id) > dir "/" doc_id ".tar.gz"

  run("ln -s "webroot"/ps3s/"doc_id".ps3.gz "dir)
  printf("this will be documentation for %s in PS 300dpi\n",
    doc_id) > webparent"/ps3s/"doc_id".ps3.gz"

  run("ln -s "webroot"/ps6s/"doc_id".ps6.gz "dir)
  printf("this will be documentation for %s in PS 600dpi\n",
    doc_id) > webparent"/ps6s/"doc_id".ps6.gz"

  run("sed s/PRODUCT/"doc_id"/ <"src"/doc/html/webprod1.htm >"index_html)

  one(".", doc_id)

  run("cat " src "/doc/html/webprod2.htm >> " index_html)
}
'

cat $src/sh/dtweb1.awk

cat $src/sh/dtpage.awk
) > $awk
}

# ---------------------------------------------------------------------------
# read the itemlist database and make secure/admin/alg/index.html pages

src=/usr/erb/prj/dt
. $src/sh/dtwebenv.sh

awk=/tmp/dtwebalg.$$.awk
data=$src/doc/products.dat
dtpage=/tmp/dtwebalg.$$.dtpage

sh $src/sh/dtdocm4.sh $src
cat $src/doc/doclist.m4 $data | \
sh $src/sh/dtinclude.sh > $dtpage

_makeawk

cat $src/doc/doclist.m4 $src/doc/itemlist.dat | \
sh $src/sh/dtinclude.sh | \
sort -k2 | \
awk -v form=html \
    -v src=$src \
    -v doc=$doc \
    -v web=$web \
    -v webparent=$webparent \
    -v webroot=$webroot \
    -v secure=$secure \
    -v admin=$admin \
    -v dtpage=$dtpage \
    -v DT_SH=$DT_SH \
    -f /tmp/dtwebalg.$$.awk $dtpage -

