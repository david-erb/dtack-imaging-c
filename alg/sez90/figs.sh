# Bourne shell script to create dt_sez90 document figures in EPS
# USAGE: figs.sh dirname
# where dirname is name of directory containing toucan.sh
#
# Support required for this script:
# dtview	holds temporary images in memory
# dtu		does basic image manipulation and processing
# dt_esc94	does error diffusion for bilevel printing
#
# Print-ready output files are all binary 1.70 inch-wide 600 dpi.
# this script creates the following output files:
# toucan.ps	EPS file ready for inclusion in LaTeX
# toucan.lj6    HP Laserjet III file ready for raw output to printer
#
# EPS is Encapsulated PostScript, 
# which can be included in LaTeX using epsf style.
#

set -xe
export DT_DIALOG
DT_DIALOG=none
#					put it into dtview for speed
dtu.d copy $1/toucan.pgm :a
#                                       double size of it to about 1000 wide
dtu.d resample -op bilinear :a .5 .5 :a
#                                       do error diffusion
dt_esc94.d -fo 0 -ba 255 :a :b
#                                       reformat as pbm file
dtu.d copy -prec b01 :b tmp.pbm
#                                       reformat as EPS file
pnmtops -noturn -dpi 600 -scale 0.125 \
  <tmp.pbm >toucan.ps
#                                       reformat as HP laserjet file
dtu.d copy -prec b01 :b toucan.lj6
#                                       get rid of tmp files
rm tmp.*
#                                       
