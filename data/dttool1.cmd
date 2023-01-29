# dttool no windows
# must be run from source directory

dir=$1					# directory where bin subdir is
out=$2					# where to put output images
exe=${3:-""}				# executable suffix

dttool$exe -h				# short help
dttool$exe -H				# long help
dttool$exe stats data/dice128.pgm	# compute and print image statistics
dttool$exe resample data/dice128.pgm \
  0.5 0.5 $out/dice256.pgm		# double image size by replication
  
dttool$exe resample -op bilinear \
  data/dice128.pgm 0.5 0.5 \
  $out/dice256.pgm			# double image size by interpolation

dttool$exe copy data/dice128.pgm \
  $out/dice128.pgm			# copy image unchanged
dttool$exe copy -prec short \
  data/dice128.pgm  $out/dice128.pgm	# copy and change to signed short
dttool$exe copy -prec double \
  data/dice128.pgm $out/dice128.pgm	# copy and change to double
dttool$exe regray $out/dice128.pgm \
  $out/dice128.pgm			# copy and expand to full dynamic range

dttool$exe gen color256 \
  $out/color256.pgm			# generate synthetic image of squares

dttool$exe gen -w 40 -h 16 \
  -background 0 \
  constant $out/inset.pgm		# make a 40x16 black image
dttool$exe paste -x 108 -y 120 \
  $out/inset.pgm $out/color256.pgm \
  $out/color256.pgm			# paste small black image in big one
dttool$exe text -x 108 -y 122 \
  $out/color256.pgm "text!" \
  $out/color256.pgm			# draw text inside image

dttool$exe box -color black \
  $out/color256.pgm \
  106 118 149 137 \
  $out/color256.pgm			# draw box around text

dttool$exe stats $out/color256.pgm





