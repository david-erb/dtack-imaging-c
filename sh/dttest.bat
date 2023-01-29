@echo off
rem must be run from source directory

if "%1"=="" goto badarg
echo =====================================          >  %1\test.out

set DT_ERRTO_OLD=%DT_ERRTO%
set DT_DBGTO_OLD=%DT_DBGTO%
set DT_DATADIR_OLD=%DT_DATADIR%
set DT_DBGTO=%1\tmp\tmp.out
set DT_ERRTO=%1\tmp\tmp.out
set DT_DATADIR=.\data

set DT_D_OLD=%DT_D%
set DT_DIALOG_OLD=%DT_DIALOG%

set DT_D=assert
set DT_DIALOG=none

call sh\dttest1.bat dtt1d       %1 dtt1d

call sh\dttest1.bat dttstr      %1 dttlow str
call sh\dttest1.bat dttheap     %1 dttlow heap
call sh\dttest1.bat dttparam    %1 dttlow param
call sh\dttest1.bat dttlist     %1 dttlow list
call sh\dttest1.bat dttos       %1 dttlow os      -output %1\tmp

rem call sh\dttest1.bat dttnote     %1 dttinter note
rem call sh\dttest1.bat dttdia      %1 dttinter dia
rem call sh\dttest1.bat dttalpha    %1 dttinter alpha -output %1\tmp

call sh\dttest1.bat dttmem      %1 dttmem mem     -output %1\tmp
call sh\dttest1.bat dttstack    %1 dttmem stack   -output %1\tmp

call sh\dttest1.bat dttstat     %1 dtthigh2 stat
call sh\dttest1.bat dttcooc     %1 dtthigh2 cooc
call sh\dttest1.bat dttjoint    %1 dtthigh2 joint
call sh\dttest1.bat dttresample %1 dtthigh2 resample
call sh\dttest1.bat dttmatrix   %1 dtthigh2 matrix

call sh\dttest1.bat dtthist     %1 dtthigh1 hist
call sh\dttest1.bat dttplot     %1 dtthigh1 plot
call sh\dttest1.bat dttlog      %1 dtthigh1 log
call sh\dttest1.bat dttpgm      %1 dtthigh1 pgm
call sh\dttest1.bat dttrun      %1 dtthigh1 run

call sh\dttest1.bat dttadj      %1 dttadj
call sh\dttest1.bat dttarith    %1 dttarith
call sh\dttest1.bat dttcast     %1 dttcast
call sh\dttest1.bat dttcmp      %1 dttcmp
call sh\dttest1.bat dttconv     %1 dttconv

call sh\dttest1.bat dttxchg     %1 dttxchg  -output %1\tmp

call sh\dttest1.bat dttscr      %1 dttscr
call sh\dttest1.bat dttview     %1 dttview  -output %1\tmp

:done

set DT_D=%DT_D_OLD%
set DT_DIALOG=%DT_DIALOG_OLD%

set DT_D_OLD=
set DT_DIALOG_OLD=

set DT_DATADIR=%DT_DATADIR_OLD%
set DT_DBGTO=%DT_DBGTO_OLD%
set DT_ERRTO=%DT_ERRTO_OLD%
set DT_DATADIR_OLD=
set DT_DBGTO_OLD=
set DT_ERRTO_OLD=

goto bottom
rem =====================================================
:badarg
echo usage: dttest.bat out_dir
goto bottom

:bottom
