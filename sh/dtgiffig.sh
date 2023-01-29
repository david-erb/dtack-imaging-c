#! /bin/sh
# scan html files in directory, changing figures to gifs and thumbnails
# ---------------------------------------------------------------------------
run()
{
  if test "$DT_SH" = "1"
  then
    echo "dtgiffig.sh: $@"
  fi
  eval "$@"
  if test ! $?
  then
    echo "dtgiffig.sh: exiting"
    exit 1
  fi
}
# ---------------------------------------------------------------------------
# scan for lines of form: 
#   <img align=top src="x/y.pgm.gz">
giffig()
{
  echo "dtgiffig.sh: $file"
  rm -f /tmp/dtgiffig.log
  pgmdir=$docdir/pgm
  gifdir=$docdir/gif
  docname=${2:-$file}
  echo "dtgiffig.sh: docname '$docname'"
  docname=`echo $docname | sed 's/[.][^.]*$//'`
  echo "dtgiffig.sh: docname '$docname'"

  if [ ! -d $gifdir/$docname ]
  then
    mkdir $gifdir/$docname
  fi
  awk -v pgmdir=$pgmdir \
      -v gifdir=$gifdir \
      -v docname=$docname \
  '
    BEGIN { FS = "\"" 
      logfile = "/dev/stderr"
      tmpfile = "/tmp/dtgiffig.size"
    }
    /.*pgm[.]gz["].*/ {
      split($2, a, ".")
      image = a[1]
      small = image "-"
      full = gifdir "/" docname "/" image ".gif"
      display = gifdir "/" docname "/" image "-.gif"
      if (system("test -r " full) ||
          system("test -r " display))
      {
        run(sprintf("zcat %s/%s/%s | dttool stats -dbgto stdout stdin > %s", 
          pgmdir, docname, $2, tmpfile))
        getline line < tmpfile
        w = substr(line, 29)
        getline line < tmpfile
        h = substr(line, 29)
        close(tmpfile)
        wscale = w / 128
        hscale = h / 128
        if (wscale > hscale)
          scale = wscale
        else
          scale = hscale
        printf(  "  %-16s w %d, h %d scale %f\n", image":", w, h, scale) >> logfile

        if (w > 128 || h > 128)
        {
          if (system("test -r " display))
        	  run(sprintf(\
              "zcat < %s/%s/%s.pgm.gz |"\
              " dttool resample -op bilinear"\
              "   stdin %f %f stdout |"\
              " ppmtogif > %s",
              pgmdir, docname, image,
              scale, scale, display))
        }
        else
        {
          display = full
          small = image
        }

        if (system("test -r " full))
      	  run(sprintf(\
            "zcat < %s/%s/%s.pgm.gz |"\
            " ppmtogif > %s",
            pgmdir, docname, image, full))
      }

      printf("<a href=\"gif/%s/%s.gif\">\n", docname, image)
      printf("<img align=top src=\"gif/%s/%s.gif\""\
        " alt=\"[gif/%s/%s.gif]\">\n",
        docname, small, 
        docname, small)
      printf("</a>\n")
      next
    }
    {print $0}

    function run(command)
    {
      print "dtgiffig: " command >> logfile
      system(command)
    }

  ' < $file > /tmp/$file
  mv /tmp/$file $file
  if [ -r /tmp/dtgiffig.log ]
  then
    cat /tmp/dtgiffig.log
  fi
}
# ---------------------------------------------------------------------------

if test $# -ne 1 -a $# -ne 2
then
  echo "dtgiffig.sh: usage html_dir"
  exit 1
fi

docdir=/usr/erb/prj/dtdoc/doc
if [ ! -d $docdir/gif ]
then
  mkdir $docdir/gif
fi

cd $1
for file in *.htm
do
  giffig $1 $2
done
