# to run this:
#   cd doc
#   (grep abbrev biblio.dat;cat biblio.dat)|sh ../sh/dtbiblio.sh biblio.bib
#   cat biblio.dat | sh ../sh/dtbiblio.sh want.bib want
# to latex it: 
#   cd doc
#   sed s-myfile-want- <biblio.ltx >want.ltx 
#   sh ../sh/dtlatex.sh .. want.ltx want.dvi:
# to ps it: cd doc; mydvips biblio

awk -v out_file=$1 -v want=$2 '
BEGIN {
  FS = "\t"

  header_file = "/tmp/bibhead"
  body_file = "/tmp/bibbody"

  print "" > header_file
  print "" > body_file

  print "dtbiblio.sh: want <" want ">"
  id = "dtbib.sh"
  added = ++n
  field[id = ++n] = "id"
  field[author = ++n] = "author"
  field[title = ++n] = "title"
  field[journal = ++n] = "journal"
  field[publisher = n] = "publisher"
  field[year = ++n] = "year"
  field[status = ++n] = "status"
  field[keywords = ++n] = "keywords"
  field[rating = ++n] = "rating"
  field[related = ++n] = "related"
  field[type = ++n] = "type"
  field[notes = ++n] = "notes"
  field[volume = ++n] = "volume"
  field[number = ++n] = "number"
  field[edition = n] = "edition"
  field[date = ++n] = "date"
  field[pages = ++n] = "pages"

  abbrev["unknown"] = "unknown"
}

{
  for (i=1; i<n; i++)
    f[i] = ""
  split($0, f)
	
  if (f[year] == "unknown")
    f[year] = 1990;

  if (f[type] == "abbreviation")
  {
    abbrev[f[id]] = f[title]
	typed = 1
  }
  else
  if (want == "" || match(f[status], want))
  {
    if (abbrev[f[journal]])
       f[journal] = abbrev[f[journal]]
    nselected++
	typed = 0;
  }
  else
    next
}


										/* ................................ */

f[type] == "misc" ||
f[type] == "report" { 
  typed = 1
}

										/* ................................ */
f[type] == "article" { 
  typed = 1
  required(author)
  required(title)
  required(journal)
  required(year)
  printf("@article{%s,\n", f[id])                     >> body_file
  printf("  author     = \"%s\",\n", f[author])       >> body_file
  printf("  title      = \"%s\",\n", f[title])        >> body_file
  printf("  journal    = \"%s\",\n", f[journal])      >> body_file
  printf("  year       = %s,\n",     f[year])         >> body_file
  if (f[volume])
  printf("  volume     = \"%s\",\n", f[volume])       >> body_file
  if (f[number])
  printf("  number     = \"%s\",\n", f[number])       >> body_file
  if (f[date])
  printf("  date       = %s,\n",     f[date])         >> body_file
  if (f[pages])
  printf("  pages      = \"%s\",\n", f[pages])        >> body_file
  printf("}\n")                                       >> body_file
}

										/* ................................ */

f[type] == "book" {
  typed = 1
  required(author)
  required(title)
  required(publisher)
  required(year)
  printf("@book{%s,\n", f[id])                        >> body_file
  printf("  author     = \"%s\",\n", f[author])       >> body_file
  printf("  title      = \"%s\",\n", f[title])        >> body_file
  printf("  publisher  = \"%s\",\n", f[publisher])    >> body_file
  printf("  year       = %s,\n",     f[year])         >> body_file
  if (f[volume])
  printf("  volume     = \"%s\",\n", f[volume])       >> body_file
  if (f[edition])
  printf("  edition    = \"%s\",\n", f[edition])      >> body_file
  printf("}\n")                                       >> body_file
}

										/* ................................ */

f[type] == "incollection" {
  typed = 1
  required(author)
  required(title)
  required(publisher)
  required(year)
  split(f[author], tmp, "::")
  fauthor = tmp[1]  
  feditor = tmp[2]  
  split(f[title], tmp, "::")
  ftitle = tmp[1]  
  fbooktitle = tmp[2]  
  printf("@incollection{%s,\n", f[id])                >> body_file
  printf("  author     = \"%s\",\n", fauthor)         >> body_file
  printf("  editor     = \"%s\",\n", feditor)         >> body_file
  printf("  title      = \"%s\",\n", ftitle)          >> body_file
  printf("  booktitle  = \"%s\",\n", fbooktitle)      >> body_file
  printf("  publisher  = \"%s\",\n", f[publisher])    >> body_file
  printf("  year       = %s,\n",     f[year])         >> body_file
  if (f[volume])
  printf("  volume     = \"%s\",\n", f[volume])       >> body_file
  if (f[edition])
  printf("  edition    = \"%s\",\n", f[edition])      >> body_file
  printf("}\n")                                       >> body_file
}

										/* ................................ */

f[type] == "manual" {
  typed = 1
  required(title)
  printf("@manual{%s,\n", f[id])                      >> body_file
  printf("  title        = \"%s\",\n", f[title])      >> body_file
  if (f[author])
  printf("  organization = \"%s\",\n", f[author])     >> body_file
  if (f[volume])
  printf("  edition      = \"%s\",\n", f[volume])     >> body_file
  if (f[year])
  printf("  year         = %s,\n",     f[year])       >> body_file
  if (f[date])
  printf("  month        = %s,\n",     f[date])       >> body_file
  printf("}\n")                                       >> body_file
}

										/* ................................ */

typed == 0 {
  printf("dtbiblio.sh: invalid type \"%s\"\n", f[type]);
}


END {
  print "dtbiblio.sh: selected " nselected
  print "dtbiblio.sh: cat " header_file " " body_file " > " out_file
  system("cat " header_file " " body_file " > " out_file)
  system("rm " header_file)
  system("rm " body_file)
}

function required(req)
{
  if (f[id] == "")
  {
    print "dtbiblio.sh: missing id field at record" NR
    next
  }
  else
  if (f[req] == "")
  {
    print "dtbiblio.sh: missing required field <" field[req] "> for id <" $id ">"
    f[req] = "\"unknown\""
  }
}
'
