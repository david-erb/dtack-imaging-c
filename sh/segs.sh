# display a bunch of small rmaps output from bu-pgms/segment

ls tmp.rmap.* | sort -t. -k3 -n |
gawk '
BEGIN {
  w = 128
  h = 120
  cmd = "(echo P5 %d %d 255; /b/ipw/bin/rmhdr %s) | \
         dttool regray -x %d -y %d stdin :%s"
}

{
  tmp = sprintf(cmd, w, h, $1, x, y, $1)
  print(tmp)
  system(tmp)
  x += w + 6
  if (x > 980)
  {
    y += h + 26
    x = 0
  }
}

END {
}'