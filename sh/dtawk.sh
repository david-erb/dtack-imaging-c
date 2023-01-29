awkfile=$1.awk
shift
gawk -f awk/util.awk -f awk/$awkfile -- $@
