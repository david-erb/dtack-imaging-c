@if "%1"=="" goto nodisk
@set jdrive=%1
:nodisk
@doskey
@k:
@cd \prj
@d:
@cd \prj
@call k:bat\dtenv msvc15 d:
@set V_SRC=k:\prj\v
@set V_OUT=d:\prj\v.wv0
@set DT_DATADIR=d:\prj\data
@set DT_SRC=k:\prj\dt
@set DT_OUT=d:\prj\dt.wv0
@set DT_TARGET=wv0
