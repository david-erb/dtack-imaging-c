BEGIN {FS = "."}
{
  old = $1
  new = "dtack/" substr(old, 3)
#  print "mv " old " " new
#  printf("echo '#include <%s>' > %s\n", new, old) 
  printf("s,<%s[.],<%s.,g\n", old, new)
}
