input=":a"
thresh="10"
binary=":t"

while [ $thresh -lt 256 ]
do
  dtu thresh $input $thresh $binary
  x=`dtu lev85_cmd -D4 -background 1 $input $binary`
  echo $thresh $x
  thresh=`echo $thresh + 10 | bc`
done
