# to run this: sh sh/dtbibfix.sh doc/biblio.dat
# to keep it: mv doc/biblio.dat.new doc/biblio.dat
# to ps it: cd doc; mydvips biblio

awk '
BEGIN {
  FS = "\t"
  in_file = ARGV[1]
  out_file = in_file ".new"

  print "dtbiblio.sh: in_file <" in_file "> out_file <" out_file ">"
  id = "dtbibfix.sh"
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
  field[rsvd = ++n] = "rsvd"
}

{
  split($0, f)
}
										/* ................................ */
f[type] == "article" {
  required(author)
  required(title)
  required(journal)
  required(year)
}

										/* ................................ */

f[type] == "book" {
  required(author)
  required(title)
  required(publisher)
  required(year)
}

										/* ................................ */

f[type] == "manual" {
  required(title)
}

										/* ................................ */
f[type] == "abbreviation" {
}
										/* ................................ */
{
  for (i=1; i<=n; i++)
  if (i == n)
      printf("%s\n", f[i]) >> out_file
    else
      printf("%s\t", f[i]) >> out_file
}


END {
}

function required(req)
{
  if (f[req] == "")
    f[req] = "\"unknown\""
}
' $1
