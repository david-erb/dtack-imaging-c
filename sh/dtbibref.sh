# convert biblio.dat (Emacs forms database) into refer database
# to run this: 
#    cat biblio.dat | sh ../sh/dtbibref.sh biblio.ref
#    cat biblio.dat | sh ../sh/dtbibref.sh want.ref want
# to groff it: 
#  cat doc/biblio.ms | \
#    sed s-myfile-doc/want.ref- | groff -mgs -R -Tascii | colcrt -
# to ps it: cd doc; mydvips biblio

out=$1
want=$2

tr '\t' @ | \
sort -k 11 -t @ | \
tr @ '\t' |
awk -v out_file=$out -v want=$want '
BEGIN {
  FS = "\t"

  print ""                                 > out_file

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
}

										/* ................................ */
{
  for (i=1; i<n; i++)
    f[i] = ""
  split($0, f)
  if (f[type] == "abbreviation")
    abbrev[f[id]] = f[title]

  if (want == "" || 
	  f[status] == want)
  {
	gsub("--", "-", f[pages])
	f[date] = toupper(substr(f[date], 1, 1)) substr(f[date], 2)
	if (abbrev[f[journal]])
	  f[journal] = abbrev[f[journal]];
	nselected++;

	if (f[type] == "misc" ||
		f[type] == "report" ||
		f[type] == "abbreviation")
	{
	}
	else					
	if (f[type] == "article")
	{
	  required(author)
	  required(title)
	  required(journal)
	  required(year)
	  printf("%%L %s\n",   f[id])               >> out_file
	  printf("%%K %s\n",   f[id])               >> out_file
	  nauthors = split(f[author], authors, " and ")
      for (i=1; i<=nauthors; i++)
  	    printf("%%A %s\n", authors[i])          >> out_file
	  printf("%%T %s\n",   f[title])            >> out_file
	  printf("%%J %s\n",   f[journal])          >> out_file
	  if (f[date] && f[year])
	    printf("%%D %s %s\n", f[date], f[year]) >> out_file
	  else
	  if (f[date] && !f[year])
	    printf("%%D %s\n", f[date])             >> out_file
	  else
	  if (!f[date] && f[year])
	    printf("%%D %s\n", f[year])             >> out_file
	  
	  if (f[volume])
	    printf("%%V %s\n",   f[volume])         >> out_file
	  if (f[number])
	    printf("%%N %s\n",   f[number])         >> out_file
	  
	  printf("%%P %s\n",   f[pages])            >> out_file
	  printf("\n")                              >> out_file
	}

				
	else
	if (f[type] == "book")
	{
	  required(author)
	  required(title)
	  required(publisher)
	  required(year)
	  printf("%%L %s\n",   f[id])              >> out_file
	  printf("%%K %s\n",   f[id])              >> out_file
	  printf("%%A %s\n",   f[author])          >> out_file
	  printf("%%T %s\n",   f[title])           >> out_file
	  printf("%%I %s\n",   f[publisher])       >> out_file
	  printf("%%D %s\n",   f[year])            >> out_file
	  if (f[volume])
	  printf("%%V %s\n",   f[volume])          >> out_file
	  if (f[number])
	  printf("%%N %s\n",   f[edition])         >> out_file
	  printf("\n")                             >> out_file
	}

				
	else
	if (f[type] == "incollection")
	{
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
	  printf("%%L %s\n",   f[id])              >> out_file
	  printf("%%K %s\n",   f[id])              >> out_file
	  printf("%%A %s\n",   fauthor)            >> out_file
	  printf("%%E %s\n",   feditor)            >> out_file
	  printf("%%T %s\n",   ftitle)             >> out_file
	  printf("%%B %s\n",   fbooktitle)         >> out_file
	  printf("%%I %s\n",   f[publisher])       >> out_file
	  printf("%%D %s\n",   f[year])            >> out_file
	  if (f[volume])
	  printf("%%V %s\n",   f[volume])          >> out_file
	  if (f[number])
	  printf("%%N %s\n",   f[edition])         >> out_file
	  printf("\n")                             >> out_file
	}
					
	else
	if (f[type] == "manual")
	{
	  required(title)
	  printf("%%L %s\n",   f[id])              >> out_file
	  printf("%%K %s\n",   f[id])              >> out_file
	  if (f[author])
	  printf("%%A %s\n",   f[author])          >> out_file
	  printf("%%T %s\n",   f[title])           >> out_file
	  printf("%%I %s\n",   f[publisher])       >> out_file
	  if (f[year] || f[date])
	  printf("%%D %s%s\n", f[date], f[year])   >> out_file
	  if (f[volume])
	  printf("%%V %s\n",   f[volume])          >> out_file
	  printf("\n")                             >> out_file
	}
					
	else
      printf("dtbibref.sh: invalid type \"%s\"\n", f[type]);
  }
	
}
  
  function required(req)
  {
	if (f[id] == "")
	{
	  print "dtbibref.sh: missing id field at record" NR
	  next
	}
	else
	if (f[req] == "")
	{
	  print "dtbibref.sh: missing required field <" field[req] "> for id <" $id ">"
	  f[req] = "\"unknown\""
	}
  }
'

# cat doc/biblio.ms | \

# sed s-myfile-$tmp- | \

# groff -mgs -R -Tascii | \

# colcrt - 
