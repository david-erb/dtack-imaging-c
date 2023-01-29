nm -n -td $1 | gawk '
BEGIN {
  type["A"] = "absolute"
  type["B"] = "global uninitialized data"
  type["b"] = "local uninitialized data"
  type["D"] = "global initialized data"
  type["d"] = "local initialized data"
  type["I"] = "indirect reference"
  type["T"] = "global code"
  type["t"] = "local code"
  type["U"] = "undefined"
}
/[.]o$/ {
  printf("  %8s %8d %s %-16s %s\n", " ", $1-start, " ", "object file", object)
  start = $1
  object = $3
}

{
 split(last, a, " ")
 split($0, b, " ")
 if (NF == 2)
 {
   b[3] = b[2]
   b[2] = b[1]
   b[1] = 0
 }
 last = b[1] " " b[2] " " b[3]

 printf("0x%08x %8d %s %-16s %s\n",
    a[1], b[1]-a[1], a[2], type[a[2]], a[3])
}
' > /tmp/dtnm.$$.1
grep "object file" /tmp/dtnm.$$.1 | sort + -1 -nr > /tmp/dtnm.$$.2
cat /tmp/dtnm.$$.1 /tmp/dtnm.$$.2 > $1.map
rm /tmp/dtnm.$$.*
