# read the itemlist database and make a links page in one of several forms

src=/usr/erb/prj/dt
sh $src/sh/dtdocm4.sh $src
cat $src/doc/doclist.m4 $src/doc/itemlist.dat | \
sh $src/sh/dtinclude.sh | \
sort -k2 | \
awk -v form=$1 '
# ==============================
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
# ==============================
function merge(id, field)
{
  if (fields[id field] == "" ||
      fields[id field] == "_")
    fields[id field] = f[field]
}
# ==============================
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
# ==============================
END {
  for (i=0; i<ndocs; i++)
    if (form == "html")
      end_html(ids[i])
}
# ==============================
function end_html(doc_id)
{
  printf("| \\<p\\>\n")
  printf("| \\<a name=\"%s\"\\>\\</a\\>\n", doc_id)

  printf("| \\&#160;\\&#160;\n")
  printf("| \\<a href=\"/dtprice/all.htm#%s\"\\>\n", doc_id)
  printf("| \\<img src=\"/gifs/price.gif\" alt=\"price\"\\>\n")
  printf("| \\</a\\>\n")

#  htmltext(doc_id, sales_lit,      "%-10s", "SALES INFO")  
#  htmltext(doc_id, tech_reference, "%-8s", "TECH DOC")  
#  htmltext(doc_id, tech_summary,   "%-8s", "TECH SUM")  
#  downtext(doc_id,                 "%-9s", "DOWN LOAD")  

  htmlicon(doc_id, sales_lit,      "sales")
  htmlicon(doc_id, tech_reference, "techref")
  htmlicon(doc_id, tech_summary,   "summary")
  downicon(doc_id,                 "downld")

  printf("| \\&#160;\\&#160;\n")
  printf("| %s\n", fields[doc_id title])

}
# ==============================
function htmlicon(doc_id, field, icon)
{
  if (fields[doc_id field] != "_")
  {
    printf("| \\&#160;\\&#160;\n")
    printf("| \\<a href=\"/%s.htm\"\\>\n", fields[doc_id field])
    printf("| \\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>\n", icon, icon)
    printf("| \\</a\\>\n")
  }
  else
  {
    printf("| \\&#160;\\&#160;\n")
    printf("| \\<a href=\"/%s.htm\"\\>\n", fields[doc_id field])
    printf("| \\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>\n", "empty", icon)
    printf("| \\</a\\>\n")
  }
}
# ==============================
function downicon(doc_id, icon)
{
  if (fields[doc_id down_load] != "_")
  {
    printf("| \\&#160;\\&#160;\n")
    printf("| \\<a href=\"/dtdown.cgi?product=%s\"\\>\n", 
      fields[doc_id down_load])
    printf("| \\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>\n", icon, icon)
    printf("| \\</a\\>\n")
  }
  else
  {
    printf("| \\&#160;\\&#160;\\&#160;\n")
    printf("| \\<img src=\"/gifs/%s.gif\" alt=\"%s\"\\>\n", icon, icon)
  }
}
# ==============================
function htmltext(doc_id, field, format, string)
{
  if (fields[doc_id field] != "_")
  {
    printf("| \\<a href=\"/%s.htm\"\\>", fields[doc_id field])
    printf(format, string)
    printf("\\</a\\> ")
  }
  else
  {
    printf("| ")
    printf(format, " ")
    printf(" ")
  }
}
# ==============================
function downtext(doc_id, format, string)
{
  if (fields[doc_id down_load] != "_")
  {
    printf("| \\<a href=\"/dtdown.cgi?product=%s\"\\>", 
      fields[doc_id down_load])
    printf(format, string)
    printf("\\</a\\> ")
  }
  else
  {
    printf("| ")
    printf(format, " ")
    printf(" ")
  }
}
# ==============================
'

# > $src/doc/itemlist.m4
