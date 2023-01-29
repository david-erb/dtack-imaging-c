set -v
kqpath=/usr/erb/kq/obj/sparcgnu/
$kqpath/kqserver -x0=10  -y0=58  $1 "A" &
$kqpath/kqserver -x0=358 -y0=0   $1 "B" &
# kqserver -x0=622 -y0=0   $1 "C" &
# kqserver -x0=886 -y0=0   $1 "D" &
# kqserver -x0=358 -y0=280 $1 "E" &
# kqserver -x0=622 -y0=280 $1 "F" &
# kqserver -x0=886 -y0=280 $1 "G" &
# kqserver -x0=358 -y0=560 $1 "H" &
# kqserver -x0=622 -y0=560 $1 "I" &
# kqserver -x0=886 -y0=560 $1 "J" &
sleep 3
kqprimgrays.sh


