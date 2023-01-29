cd $1
for i in *.[chp] 
do
  if [ -r $2/$i ]
  then
    diff -s $i $2/$i > /dev/null
    rc=$?
    if [ $rc -ne 0 ]
    then
      echo $i differs
    fi
  else
    echo $i does not exist in $2
  fi
done
