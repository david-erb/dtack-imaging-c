# read the doclist database and make an m4 input file with definitions
# can't have the awk code here because of m4's goofy `' construct

awk -f $1/sh/dtdocm4.awk <$1/doc/doclist.dat > $1/doc/doclist.m4
