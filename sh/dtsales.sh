# read the itemlist database and make a sales links page in one of several forms

src=/usr/erb/prj/dt
sh $src/sh/dtdocm4.sh $src
cat $src/doc/doclist.m4 $src/doc/itemlist.dat | \
sh $src/sh/dtinclude.sh | \
sort -k2 | \
awk -v form=$1 '
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
function merge(id, field)
{
  if (fields[id field] == "" ||
      fields[id field] == "_")
    fields[id field] = f[field]
}
{
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
function end_html(doc_id)
{
  if (fields[doc_id sales_lit] == '_')
    return
  printf("| \\<a name=\"%s\"\\>\\</a\\>\n", doc_id)
  printf("| \\<p\\>\n")
  links(doc_id)
  printf("| \\<a href=\"/%s/all.htm\"\\> #%s# \\</a\\>\n", 
    fields[doc_id sales_lit], fields[doc_id title])

}
function links(doc_id)
{
    printf("| | \\<a href=\"/dtlinks/all.htm#%s\"\\>LINKS\\</a\\> ", doc_id)

    printf("| \\<a href=\"../dtprice/all.htm#%s\"\\>PRICE\\</a\\> ", doc_id)

    link = fields[doc_id down_load]
    if (link != "_")
      printf("| \\<a href=\"/dtdown/all.htm#%s\"\\>DOWN LOAD\\</a\\> ", doc_id)
}

END {
}
'

# > $src/doc/itemlist.m4
