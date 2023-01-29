#! /bin/bash

# Demo for GEMS 6/25/97

# a. display raw iqphant.img 512x512 image (dim image)
#    display histogram
#    print statistics (size, min, max, mean)
# pause
# b. linear stretch contrast and show result (brighter)
#    display histogram
# pause
# c. convolve bright image with 3x3 Laplace and display (black edges)
# pause
# d. bilevel threshold to black-and-white image (most outlines appear)
# pause
# e. do connected components analysis and draw outlines on image
#    (green boxes around some stuff)
# pause
# f. convolve with 3x3 averaging filter and display (blurry)
# pause
# g. grayscale erode (very blurry)
# pause
# h. multithreshold to 4 levels (posterized)
# pause
# i. do connected components analysis and draw outlines and sizes on image
#    (many interesting features identified by green boxes)
# pause
# j. draw red box around a particular ROI
#    (red box around rivet plate)
# pause
# k. extract the ROI and draw boxes around blobs, print statistics on blobs
#    (rivets identified by green boxes)
# pause

_pause()
{
  echo -n "hit enter to continue..."
  read answer
}

_say()
{
  echo "$@"
}

_server()
{
  myslaughter dtview
  cd /
  dtview &
  sleep 3
}

_prep()					# prepare raw image and luts
{
  dttool gen -w 256 -h 3 vramp pga,/tmp/gray.lut
  rawtopgm -h 512 512 512 /f/prj/iqsign/image2/iqphant.img > /tmp/raw.pgm
}

_a()					# display raw image
{
  _say "a. display raw iqphant.img 512x512 image (dim image)"
  _say "   display histogram"
  _say "   print statistics (size, min, max, mean)"
  dttool regray -x 0 -y 0 -min 8 /tmp/raw.pgm :a
  dttool hist :a :ahist 
}

_b()					# linear stretch contrast
{
  _say "b. linear stretch contrast and show result (brighter)"
  _say "   display histogram"
  dttool linear :a 1.8 8 :b
  dttool hist :b :bhist 
}

_c()					# convolve Laplace
{
  _say "c. convolve bright image with 3x3 Laplace and display (black edges)"
  dttool convolve -D img -precision s4 :b laplace /tmp/tmp.pgm
  dttool regray -precision b08 -min 8 /tmp/tmp.pgm :c
}

_d()					# bilevel threshold
{
  _say "d. bilevel threshold to black-and-white image (most outlines appear)"
  dttool thresh :c 100 :d
}

_e()					# draw boxes around blobs
{
  _say "e. do connected components analysis and draw outlines on image"
  _say "   (green boxes around some stuff)"
  dttool enclose -min 20 -max 10000 -op enc -conn 8 -D stats :d :d :e  
}

_f()					# average convolve
{
  _say "f. convolve with 3x3 averaging filter and display (blurry)"
  dttool convolve -D img -precision s4 :b ave /tmp/tmp.pgm
  dttool regray -precision b08 -min 8 /tmp/tmp.pgm :f
}

_g()					# erode
{
  _say "g. grayscale erode (very blurry)"
  dttool morph -op erode :f ave :g
}

_h()					# multithreshold
{
  _say "h. multithreshold to 4 levels (posterized)"
  dttool multithresh :g 50 100 150 200 :h
}

_i()					# draw boxes around blobs
{
  _say "i. do connected components analysis and draw outlines and sizes on image"
  _say "   (many interesting features identified by green boxes)"
  dttool enclose -min 150 -max 10000 -op num+enc -conn 8 -D stats :h :h :i
}

_j()					# draw red box around ROI
{
  _say "j. draw red box around a particular ROI"
  _say "   (red box around rivet plate)"
  dttool box -color 3 :h 110 172 175 315 :j
  dttool box -color 3 :j 109 171 176 316 :j
}

_k()					# extract ROI and draw/print blobs
{
  _say "k. extract the ROI and draw boxes around blobs, print statistics on blobs"
  _say "   (rivets identified by green boxes)"
  dttool copy -dx 110 -dy 172 -w65 -h 142 :h :k
  dttool enclose -min 10 -max 1000 -op enc -conn 8 -D stats :k :k :k
}

_all()
{
  _server
  _pause
  _prep
  _a
  _pause
  _b
  _pause
  _c
  _pause
  _d
  _pause
  _e
  _pause
  _f
  _pause
  _g
  _pause
  _h
  _pause
  _i
  _pause
  _j
  _pause
  _k
  _pause
  echo "the end."
}
# --------------------------------------------------------------------------------

export DISPLAY=localhost:0.0

eval _$1

