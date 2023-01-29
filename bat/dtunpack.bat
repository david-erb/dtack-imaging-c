@echo off
rem unpack package for target given on command line
rem if second argument is exactly "inst", then calls dtinst after unpack

call dtset %1
if "%DT_BATCHERR%"=="" goto dtsetok
goto error
:dtsetok

dtutil time dtunpack: _date_ _time_ %DT_ID% unzipping...
pkunzip -o -d \prj\dt.zip > pkunzip.out

if "%2"=="inst" goto inst
goto done

:inst
call bat\dtinst
goto done

rem =====================================================
:done

rem =====================================================
:exit

