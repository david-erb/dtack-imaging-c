# read the itemlist database and make a links page in one of several forms

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
  if (fields[doc_id sales_lit] == "_" &&
      fields[doc_id tech_reference] == "_" &&
      fields[doc_id tech_summary] == "_" &&
      fields[doc_id down_load] == "_")
    return
  printf("| \\<a name=\"%s\"\\>\\</a\\>\n", doc_id)
  printf("| #%s#\n", fields[doc_id title])
  printf("| \\&#160;\n")
  printf("| \\&#160;\n")
  printf("| \\&#160;\n")
  printf("| \\&#160;\n")
  printf("| \\&#160;\n")
  links(doc_id)
  printf("|\n")

  printf("| \\<pre\\>\n");
  html(doc_id, sales_lit,      "Sales Information")  
  html(doc_id, tech_reference, "Technical Reference")
  html(doc_id, tech_summary,   "Technical Summary")
  printf("| \\</pre\\>\n");
}
function html(doc_id, field, string)
{
  link = fields[doc_id field]
  if (link != "_")
  {
    printf("| %-24s", string)
    printf("| \\<a href=\"/%s/htm\"\\>%s\\</a\\> ",  link,  "browse");
    printf("| \\<a href=\"/dttext/%s.txt\"\\>%s\\</a\\> ", doc_id,"text");
    printf("| \\<a href=\"/dtps3/%s.ps3\"\\>%s\\</a\\> ",  doc_id,"PS 300dpi");
    printf("| \\<a href=\"/dtps6/%s.ps6\"\\>%s\\</a\\> ",  doc_id,"PS 600dpi");
    printf("|\n")
  }
}
function links(doc_id)
{
    printf("| | \\<a href=\"/dtlinks/all.htm#%s\"\\>LINKS\\</a\\> ", doc_id)

    printf("| \\<a href=\"/dtprice/all.htm#%s\"\\>PRICE\\</a\\> ", doc_id)

    link = fields[doc_id down_load]
    if (link != "_")
      printf("| \\<a href=\"/dtdown/all.htm#%s\"\\>DOWN LOAD\\</a\\> ", doc_id)
}

END {
}
'

# > $src/doc/itemlist.m4
