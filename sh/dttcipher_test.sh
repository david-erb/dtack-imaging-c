ls -lt data/runadj.dat

dttcipher -op encrypt -in data/runadj.dat -out /tmp/encrypt.out -n 100 -D time
ls -lt /tmp/encrypt.out

dttcipher -op decrypt -in /tmp/encrypt.out -out /tmp/decrypt.out -n 100 -D time
ls -lt /tmp/decrypt.out

diff data/runadj.dat /tmp/decrypt.out
rc=$?

echo "diff rc $rc"


