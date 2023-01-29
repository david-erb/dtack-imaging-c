# sample programs not needing windows
# must be run from source directory

dir=$1					# directory where bin subdir is
out=$2					# where to put output images
exe=${3:-""}				# executable suffix

dtenv1$exe

dtenv2$exe -color blue			# params and args
dtenv2$exe -color blue yyy
dtenv2$exe xxx

dtenv3$exe				# yield for 5 seconds

dtenv4$exe				# error output mechanism

dtexim1$exe $out/dtexim1.pgm		# generate, export, import
dtexim1$exe $out/dtexim1.bmp
dtexim1$exe $out/dtexim1.wmf
dtexim1$exe $out/dtexim1.pga

dthist1$exe
dthist2$exe

dtmem1$exe
dtmem2$exe

dtparam1$exe
