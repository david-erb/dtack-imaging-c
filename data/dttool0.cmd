# dttool helps
# must be run from source directory

dir=$1					# directory where bin subdir is
out=$2					# where to put output images
exe=${3:-""}				# executable suffix

dttool$exe -h				# short help
dttool$exe -H				# long help

dttool$exe box -h	
dttool$exe box -H

dttool$exe copy -h	
dttool$exe copy -H

dttool$exe convolve -h	
dttool$exe convolve -H
