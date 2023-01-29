#
# ------------------------------------------------------------
_say()
{
  echo "pump.sh: $@" 
}
# ------------------------------------------------------------
_error()
{
  _say "ERROR $@"
  exit
}
# ------------------------------------------------------------
_run()
{
  cmd=`echo "$@" | tr -s ' '`		# make printed line prettier
  _say "$cmd" 1>&2
  eval "$cmd"
  rc=$?
  if [ $rc -ne 0 ]
  then
    _error "rc=$rc doing \"$cmd\""
  fi
}

# ------------------------------------------------------------
_init()
{
  _run "$bin/dttfb \
    -dialog none \
    -op reset \
    -D assert \
    psi,0xad6,0xd0000,/vmc2/config/vmc2.cfg"
}


# ----------------------------------------------------------
# grab a single frame to a raw file
_snap()
{
  _run "$bin/dttfb \
    -dialog none \
    -op overlap \
    -color y_cb_cr_411 \
    -n 1 \
    -out $dir/$frame.raw \
    -D usr1+usr2 \
    -sigma 1 \
    psi,0xad6,0xd0000,/vmc2/config/vmc2.cfg"
}

# ----------------------------------------------------------
# convert raw file to jpeg
_convert()
{
					# convert frame buffer to rgb triplets
  _run "$bin/dttool color \
    -color y_cb_cr_411 \
    -out rgb888 \
     -w 352 -h 288 \
     $dir/$frame.raw \
     raw,,$dir/$frame.rgb"
					# convert rgb triplets to ppm
  _run "$bin/rawtoppm 352 288 \
     $dir/$frame.rgb \
     > $dir/$frame.ppm"
					# convert ppm to jpeg
  _run "$bin/cjpeg -vv \
     -quality $quality \
     $dir/$frame.ppm \
     > $dir/$frame.jpg"
}

# ----------------------------------------------------------

dir=/tmp/demo00-01
bin=$dir/bin
quality=60
frame=frame

_init

while true
do
  _snap
  _convert
  ls -lt $dir/$frame.jpg
done

exit
