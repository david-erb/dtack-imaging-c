@echo off
rem lots of environment variable space needed!
rem put the following into CONFIG.SYS: SHELL=c:\dos\command.com /e:1024
rem install package for target given on command line
rem assumes unpack has already been done

rem set DT_SRC=%1
rem set DT_OUT=%2
rem set DT_TARGET=%3
rem set DT_ENV=%4

if "%1"=="" goto badargs
if "%2"=="" goto badargs
if "%3"=="" goto badargs
if "%4"=="" goto badargs

if "%1"=="%2" goto nocopy
%1\dtutil mkdir %2
copy %1\dtutil.exe %2
echo %1\dtutil cd %2
dtutil cd %2
cd
:nocopy

echo ==============================================    >  make.out
dtutil time dtinst.bat _date_ _time_ %3                >> make.out
echo ==============================================    >  error.out
dtutil time dtinst.bat _date_ _time_ %3                >> error.out
echo ==============================================    >  warning.out
dtutil time dtinst.bat _date_ _time_ %3                >> warning.out
echo ==============================================    >  fail.out
dtutil time dtinst.bat _date_ _time_ %3                >> fail.out
echo ==============================================    >  timing.out
dtutil time dtinst.bat _date_ _time_ %3                >> timing.out

dtutil time dtinst.bat _date_ _time_ dtmkdirs.bat %2
dtutil time dtinst.bat _date_ _time_ dtmkdirs.bat %2 >> make.out
call %1\bat\dtmkdirs.bat %2
if "%DT_BATCHERR%"=="" goto dtmkdirsok
goto done
:dtmkdirsok

dtutil time dtinst.bat _date_ _time_ dtenv.bat %4
dtutil time dtinst.bat _date_ _time_ dtenv.bat %4 >> make.out
call %1\bat\dtenv.bat %4
if "%DT_BATCHERR%"=="" goto dtenvok
goto done
:dtenvok

dtutil time dtinst.bat _date_ _time_ %DT_MAKE% %3.mak
dtutil time dtinst.bat _date_ _time_ %DT_MAKE% %3.mak >> make.out
%DT_MAKE% %1\%3.mak SRC=%1 OUT=%2 >> make.out
if "%DT_BATCHERR%"=="" goto dtmakeok
goto done
:dtmakeok

dtutil time dtinst.bat _date_ _time_ %3 stripping...
%DT_MAKE% %1\%3.mak SRC=%1 OUT=%2 strip >> make.out
del bin\*.map >> make.out

if "%1"=="%2" goto nocd
echo dtutil cd %1 >> make.out
dtutil cd %1
:nocd

dtutil time dtinst.bat _date_ _time_ %3 testing...
call bat\dttest %2

goto exit

dtutil time dtinst.bat _date_ _time_ %3 timing...
call bat\dttiming %2

goto exit

rem =====================================================
:error
dtutil time dtinst.bat _date_ _time_ %3 %DT_BATCHERR% >> make.out
goto bottom

rem =====================================================
:badargs
echo dtinst.bat: invalid arguments
goto bottom

rem =====================================================
:exit

dtutil time dtinst.bat _date_ _time_ %3 checking...
call %1\bat\dtcheck %2

dtutil time dtinst.bat _date_ _time_ %3 complete >> %2\make.out
dtutil time dtinst.bat _date_ _time_ %3 complete >> %2\error.out
dtutil time dtinst.bat _date_ _time_ %3 complete >> %2\warning.out
dtutil time dtinst.bat _date_ _time_ %3 complete >> %2\fail.out
dtutil time dtinst.bat _date_ _time_ %3 complete >> %2\timing.out

:done

dtutil time dtinst.bat _date_ _time_ dtenv.bat %4_unset
dtutil time dtinst.bat _date_ _time_ dtenv.bat %4_unset >> %2\make.out
call %1\bat\dtenv.bat %4_unset
:bottom
