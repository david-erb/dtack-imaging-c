@echo off
if "%1"=="register"     goto register
if "%1"=="unregister"   goto unregister
if "%1"=="pack"         goto pack
if "%1"=="make"         goto make
echo mjdll: usage: [register unregister pack]
goto exit
rem ---------------------------------------------------
:register
@echo on
regsvr32 /c /s dtmj_grabit.ax
regsvr32 /c /s dtmj_encode.ax
regsvr32 /c /s dtmj_cgiout.ax
@echo off
goto done
rem ---------------------------------------------------
:unregister
@echo on
regsvr32 /u /s dtmj_grabit.ax
regsvr32 /u /s dtmj_encode.ax
regsvr32 /u /s dtmj_cgiout.ax
@echo off
goto done
rem ---------------------------------------------------
:pack
@echo on
cvpack /nologo /strip dtmj_grabit.ax
cvpack /nologo /strip dtmj_encode.ax
cvpack /nologo /strip dtmj_cgiout.ax
@echo off
goto done
rem ---------------------------------------------------
:make
@echo on
call k:bat\makemj.bat jpeg.mak
call k:bat\makemj.bat grabit.mak
call k:bat\makemj.bat encode.mak
call k:bat\makemj.bat cgiout.mak
@echo off
goto done
rem ---------------------------------------------------
:done
echo mjdll: %1 done
goto exit
rem ---------------------------------------------------
:exit
