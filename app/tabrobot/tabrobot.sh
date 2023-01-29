# used on moist
# port of where we listen on
# ipaddr of where tabptz is running on port 5791

port=$1
ipaddr=$2

date=`date +%Y%m%d.%H%M`
log=logs/tabrobot.$ipaddr.log

if [ $# -ne 2 ]
then
  echo "usage: wrong number of arguments"
  exit 1
fi

set -x
tabrobot \
  -connect rocket,207.124.103.109:$port \
  -output ipc,rocket,$ipaddr:5791 \
  -config presets/$ipaddr.presets \
  -D usr1+usr2+command \
  -errto $log \
  -dbgto $log \
  -sayto $log &
