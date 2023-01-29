@echo off
rem must be run from source directory

if "%1"=="" goto badarg

set DT_OUT_OLD=%DT_OUT%
set DT_OUT=%1
echo =====================================          >  %DT_OUT%\timing.out

set DT_D_OLD=%DT_D%
set DT_ERRTO_OLD=%DT_ERRTO%
set DT_DBGTO_OLD=%DT_DBGTO%
set DT_DATADIR_OLD=%DT_DATADIR%
set DT_DIALOG_OLD=%DT_DIALOG%

set DT_D=assert+time
set DT_ERRTO=%DT_OUT%\tmp\tmp.out
set DT_DBGTO=%DT_OUT%\tmp\tmp.out
set DT_DATADIR=.\data
set DT_DIALOG=none

call sh\dttimin1.bat dtt1d    %DT_OUT% dtt1d
call sh\dttimin1.bat dttmem   %DT_OUT% dttmem mem   -output %DT_OUT%\tmp
call sh\dttimin1.bat dttstack %DT_OUT% dttmem stack -output %DT_OUT%\tmp

call sh\dttimin1.bat dttarith %DT_OUT% dttarith -n 50 -reps 1

call sh\dttimin1.bat dttcast  %DT_OUT% dttcast -n 50 -reps 1
call sh\dttimin1.bat dttlog   %DT_OUT% dtthigh1 log -n 50
call sh\dttimin1.bat dttconv  %DT_OUT% dttconv
call sh\dttimin1.bat dttrun   %DT_OUT% dtthigh1 run

call sh\dttimin1.bat dttadj   %DT_OUT% dttadj

:restore
set DT_D=%DT_D_OLD%
set DT_ERRTO=%DT_ERRTO_OLD%
set DT_DBGTO=%DT_DBGTO_OLD%
set DT_DATADIR=%DT_DATADIR_OLD%
set DT_DIALOG=%DT_DIALOG_OLD%

set DT_D_OLD=
set DT_ERRTO_OLD=
set DT_DBGTO_OLD=
set DT_DATADIR_OLD=
set DT_DIALOG_OLD=

set DT_OUT=%DT_OUT_OLD%
set DT_OUT_OLD=
goto bottom
rem =====================================================
:badarg
echo usage: dttiming.bat <compiled_dt_dir> 
goto bottom

:bottom
