k:
cd \prj\dt
d:
cd \prj\dtwv4
call k:bat\b2 fourth\jpeg6a libs
k:
cd com\jpeg1
nmake -f jpeg.mak
nmake -f encode.mak
nmake -f decode.mak
cd ..\..
