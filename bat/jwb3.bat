@set jdrive=d:
if "%1"=="" goto nodisk
@set jdrive=%1
:nodisk
@doskey
@k:
@cd \prj\dt
@%jdrive%
@cd \prj\dtwb3
@call k:bat\dtenv bc45w32 %jdrive%
@set DT_DATADIR=%jdrive%\prj\data
@set DT_SRC=k:\prj\dt
@set DT_OUT=%jdrive%\prj\dtwb3
@set DT_TARGET=wb3
