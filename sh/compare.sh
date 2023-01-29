input=":a"
thresh="10"
binary=":t"

while [ $thresh -lt 256 ]
do
  dtu thresh $input $thresh $binary
  dtu blob -Dc -op compare $binary
  thresh=`echo $thresh + 10 | bc`
done
