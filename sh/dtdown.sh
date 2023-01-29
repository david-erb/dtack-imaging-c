# read the itemlist database and make a download page in one of several forms

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
  printf("| \\<form action=\"/dtdown.cgi\" method=GET\\>\n");
  printf("| \\<select name=\"product\" size=%d\\>\n", ndocs);
  for (i=0; i<ndocs; i++)
    if (form == "html")
      end_html(ids[i])
  printf("| \\</select\\>\n");

  printf("| \\<br\\>\n");
  printf("| Please select a product from the list above.\n");
  printf("| \\<br\\>\n");
  printf("| If you select nothing, clicking SUBMIT REQUEST\n");
  printf("| will give you a list of products eligible for\n");
  printf("| your passcode.\n");

  printf("| \\<p\\>\n");
  printf("| Passcode: \\<input name=\"passcode\" value=\"\" type=\"text\"\\>\n");
  printf("| \\<br\\>\n");

  printf("| Please enter the passcode assigned to you.\n");

  printf("| \\<!-- error --\\>\n");
  printf("| \\<p\\>\n");
  printf("| \\<input type=\"submit\" value=\"SUBMIT REQUEST\"\\>\n");
  printf("| \\<input type=\"reset\" value=\"RESET INPUT\"\\>\n");
  printf("| \\</form\\>\n");

  printf("| \\<p\\>\n");
  printf("| We also offer some \\<a href=\"/dtdown.cgi?passcode=free\"\\>\n");
  printf("| \\<b\\>cost-free software\\</b\\>\\</a\\> for downloading.\n");
}
function end_html(doc_id)
{
  link = fields[doc_id down_load]
  if (link != "_")
  {
    printf("| \\<option value=\"%s\"\\>\\&#160;%s\\&#160\n",
      doc_id, fields[doc_id title])
  }
}

END {
}
'

# > $src/doc/itemlist.m4
