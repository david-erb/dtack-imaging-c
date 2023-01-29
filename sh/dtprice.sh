# read the itemlist database and make a price list in one of several forms

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
{split($0, f)}
#------------------------------------------------------------------------
form == "text" {
  printf("%s %-48s %-16s $%3d.00\n", $1, $2, $3, $4)
}
#------------------------------------------------------------------------
form == "html" {
# preformatted
# text: title    license      partno    price
# link: product  download
# text: title    printed      partno    price
# link: product  tech_ref

  if (NR <= 1)
  {
    printf("| %-40s%-24s%-8s%s\n", "ITEM", "DELIVERABLE", "PART", "  PRICE")
    printf("| %-40s%-24s%-8s%s\n", "====", "===========", "====", "  =====")
  }
  if (have[f[id]] == 0)
  {
    printf("| \\<a name=\"%s\"\\>\\</a\\>", f[id])
    have[f[id]] = 1
  }
  else
    printf("| ")
  html(title, "%-40s", f[title])
  if (f[deliverable] == "license")
    html(down_load, "%-24s", "License")
  else
    html(tech_reference,  "%-24s", "Printed Documentation")
    
  if (f[price] == "free")
    printf("%-8s  waived   ",
      f[partno])
  else
    printf("%-8s $%3s.00   ",
      f[partno], f[price])
  if (f[deliverable] == "license")
  {
    links(f[id])
    printf("|")
  }
  printf("\n")
}
function html(field, format, string)
{
  printf("f[%s] = \"%s\"\n", field, f[field]) > "/dev/stderr"
  printf(format, string)
}
function links(doc_id)
{
    link = fields[doc_id sales_lit]
    if (link != "_")
      printf("| \\<a href=\"/dtlinks/all.htm#%s\"\\>LINKS\\</a\\> ", doc_id)
    else
      printf("| %s ", "     ")

    link = fields[doc_id tech_reference]
    if (link != "_")
      printf("| \\<a href=\"/dtdoc/all.htm#%s\"\\>TECH DOC\\</a\\> ", doc_id)
    else
      printf("| %s ", "        ")

    link = fields[doc_id down_load]
    if (link != "_")
      printf("| \\<a href=\"/dtdown/all.htm#%s\"\\>DOWN LOAD\\</a\\> ", doc_id)
    else
      printf("| %s ", "        ")
}
#------------------------------------------------------------------------

END {
}
'

# > $src/doc/itemlist.m4
