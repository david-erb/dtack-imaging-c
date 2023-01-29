# to run this:
#   sh sh/dtprint.sh normal normal.out
#   sh sh/dtprint.sh finish finish.out
#   cat normal.out relax.txt finish.out > /b/tmp/print.pjl

awk -v type=$1 -v out=$2 '
BEGIN {
  ESC = 27
  printf("%cE", ESC)            >  out
  if (type == "normal")					# big HP book page 8-24
  {
    printf("%c(0U", ESC)        >> out  # symbol set, 0=ASCII
    printf("%c(s0P", ESC)       >> out  # spacing, 0=fixed
    printf("%c(s16.66H", ESC)   >> out  # pitch, cpi
    printf("%c(s8.5V", ESC)     >> out  # height, points
    printf("%c(s0S", ESC)       >> out  # style, 0=upright
    printf("%c(s1B", ESC)       >> out  # stroke weight, 0=normal, 3=bold
    printf("%c(s0T", ESC)       >> out  # typeface family, 0=lineprinter
    printf("%c&k2G", ESC)       >> out  # cr=cr, lf=cr+lf
    printf("%c&a20L", ESC)      >> out  # left margin, chars
  }
  else
  if (type == "finish")
  {
  }
  else
    printf("dtprint.sh: unknown type \"%s\"\n", type)
}
'
