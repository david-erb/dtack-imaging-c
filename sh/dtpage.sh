# ------------------------------------------------------------
_say()
{
  echo "guest.sh: $@" 
}
# ------------------------------------------------------------
_run()
{
  _say "$@" 1>&2
  eval "$@"
  if [ $? -ne 0 ]
  then
    _error "doing $@"
  fi
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit
}
# ------------------------------------------------------------

# read the itemlist database and make a links page in one of several forms

src=/usr/erb/prj/dt

form=$1
shift
if [ $# -gt 0 ]
then
  data=$1
  shift
else
  data=products
fi

data=$src/doc/${data}.dat

if [ ! -r $data ]
then
  _error "$data does not exist"
fi

sh $src/sh/dtdocm4.sh $src
cat $src/doc/doclist.m4 $data | \
sh $src/sh/dtinclude.sh | \
awk -v form=$form '
# ==============================
BEGIN { 
  FS = "\t"; 
}
# ==============================
function say(string)
{
  printf("%s\n", string) >> "/dev/stderr"
}
# ==============================
function error(string)
{
  say(string)
  exit
}
# ==============================
function line_out(string)
{
  printf("| %s\n", string)
}
# ==============================
function space(amount)
{
  line_out("\\<img src=\"/gifs/space"amount".gif\" alt=\"\"\\>")
}
# ==============================
$1 == "product" { np++; 
                  product[np] = f["product" np] = $2; ndoc[np] = 0; next }
$1 == "part"    { part[np]    = f["part" np]    = $2; next }
$1 == "title1"  { title1[np]  = f["title1" np]  = $2; next }
$1 == "title2"  { title2[np]  = f["title2" np]  = $2; next }
$1 == "lic_fee" { lic_fee[np] = f["lic_fee" np] = $2; next }
$1 == "doc_fee" { doc_fee[np] = f["doc_fee" np] = $2; next }
$1 == "doc"     { doc[np ndoc[np]] = f["doc" np ndoc[np]] = $2; ndoc[np]++; next }
NF == 0         { next }
substr($1, 1, 1) == "#" { next }
{ error("bad line " $0) }

# ==============================
END {
  line_out("\\<!-- dtpage.sh for "form" start --\\>")
  for (p=1; p<=np; p++)
  {
    nicons = 0
    if (form == "links.htm")
      links_htm(p)
    if (form == "price.htm")
      price_htm(p)
    if (form == "sales.htm")
      sales_htm(p)
    if (form == "doc.htm")
      doc_htm(p)
  }
  line_out("\\<!-- dtpage.sh for "form" finish --\\>")
}
# ==============================
function links_htm(p)
{
  html_button_link("PRICE", "dtprice", "#"product[p])
  html_button_link("SALES", product[p])
  html_button_link("TECHREF", product[p])
  html_button_cgi("DOWNLOAD", dtdown, "product="product[p])
  html_title1(title1[p])
  html_title2(title2[p])
}
# ==============================
function doc_htm(p)
{
  line_out("\\<p\\>")
  html_title1(title1[p])
say("title2["p"] = <"title2[p]">")
  html_title2(title2[p])
  for (i=0; i<ndoc[p]; i++)
  {
    space(16)
    html_button_text("browse",           doc[p i])
    html_button_text("text",             doc[p i], doc[p i]".txt")
    html_button_text("PS300",            doc[p i], doc[p i]".ps3")
    html_button_text("PS600",            doc[p i], doc[p i]".ps6")
    line_out(sprintf("dtdoclist(%s, title)", doc[p i]))
    line_out("\\<br\\>")
  }
}
# ==============================
function sales_htm(p)
{
  html_button_link("PRICE", "dtprice", "#"product[p])
  html_button_link("SALES", product[p])
  html_title1(title1[p])
  html_title2(title2[p])
}
# ==============================
function price_htm(p)
{
  html_button_link("SALES", product[p])
  html_title1(title1[p])
  line_out("\\<pre\\>")
  line_out(sprintf(".%14sLicense fee       Item L%3d    $%3d", " ", 
    part[p], lic_fee[p]))
  line_out(sprintf(".%14sPrinted docs      Item D%3d    $%3d", " ", 
    part[p], doc_fee[p]))
  line_out("\\</pre\\>")
}
# ==============================
# ==============================
function html_title1(title1)
{
  line_out(sprintf("\\<b\\>%s\\</b\\>", title1))
  line_out("\\<br\\>")
}
# ==============================
function html_title2(title2)
{
  html_button_skip(nicons)
  space(16)
  line_out(title2)
  line_out("\\<br\\>")
}
# ==============================
function html_button_link(icon, page, local)
{
  line_out(sprintf("\\<a href=\"/%s/%s\"\\>", page, local))
  line_out(sprintf("\\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>", icon, icon))
  line_out("\\</a\\>")
  space(10)
  nicons++
}
# ==============================
function html_button_cgi(icon, script, argument)
{
  line_out(sprintf("\\<a href=\"/%s?%s\"\\>", script, argument))
  line_out(sprintf("\\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>", icon, icon))
  line_out("\\</a\\>")
  space(10)
  nicons++
}
# ==============================
function html_button_text(text, page, local)
{
  line_out(sprintf("[\\<a href=\"/%s/%s\"\\>", page, local))
  line_out(sprintf("%s", text))
  line_out("\\</a\\>]")
  space(10)
  nicons++
}
# ==============================
function html_button_skip(n)
{
  for (i=0; i<n; i++)
    html_button_inert("line0", "")
}
# ==============================
function html_button_inert(icon, alt)
{
  line_out(sprintf("\\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>", icon, alt))
  space(10)
}
# ==============================
'

# > $src/doc/itemlist.m4

