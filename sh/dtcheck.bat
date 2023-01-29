@echo off
rem must be run from source directory

if "%1"=="" goto badarg

set DT_OUT_OLD=%DT_OUT%
set DT_OUT=%1
@echo off
echo =====================================          >  %DT_OUT%\error.out
dtutil time dtcheck.bat _date_ _time_ %DT_OUT%\make.out >> %DT_OUT%\error.out
dtutil time dtcheck.bat _date_ _time_ scanning %DT_OUT%\make.out ...........
dtutil grep error         %DT_OUT%\make.out         >> %DT_OUT%\error.out
dtutil grep fatal         %DT_OUT%\make.out         >> %DT_OUT%\error.out
dtutil grep failed        %DT_OUT%\make.out         >> %DT_OUT%\error.out

echo =====================================          >  %DT_OUT%\warning.out
dtutil time dtcheck.bat _date_ _time_ %DT_OUT%\make.out >> %DT_OUT%\warning.out
dtutil grep warning       %DT_OUT%\make.out             >> %DT_OUT%\warning.out

echo =====================================              >  %DT_OUT%\testfail.out
dtutil time dtcheck.bat _date_ _time_ %DT_OUT%\test.out >> %DT_OUT%\testfail.out
dtutil time dtcheck.bat _date_ _time_ scanning %DT_OUT%\test.out ...........
dtutil grep "error:"         %DT_OUT%\test.out          >> %DT_OUT%\testfail.out
dtutil grep ": *"            %DT_OUT%\test.out          >> %DT_OUT%\testfail.out
dtutil grep "run-time error" %DT_OUT%\test.out          >> %DT_OUT%\testfail.out
dtutil endcheck       %DT_OUT%\test.out                 >> %DT_OUT%\testfail.out

echo =====================================              >  %DT_OUT%\timefail.out
dtutil time dtcheck.bat _date_ _time_ %DT_OUT%\timing.out >> %DT_OUT%\timefail.out
dtutil time dtcheck.bat _date_ _time_ scanning %DT_OUT%\timing.out .........
dtutil grep "error:"         %DT_OUT%\timing.out    >> %DT_OUT%\timefail.out
dtutil grep ": *"            %DT_OUT%\timing.out    >> %DT_OUT%\timefail.out
dtutil grep "run-time error" %DT_OUT%\timing.out    >> %DT_OUT%\timefail.out
dtutil endcheck       %DT_OUT%\timing.out           >> %DT_OUT%\timefail.out

type %DT_OUT%\error.out    >  %DT_OUT%\check.out
type %DT_OUT%\warning.out  >> %DT_OUT%\check.out
type %DT_OUT%\testfail.out >> %DT_OUT%\check.out
type %DT_OUT%\timefail.out >> %DT_OUT%\check.out
dtutil time dtcheck.bat _date_ _time_ results stored in %DT_OUT%\check.out

set DT_OUT=%DT_OUT_OLD%
set DT_OUT_OLD=

goto bottom
rem =====================================================
:badarg
echo dtcheck.bat: missing OUT directory name argument
goto bottom

:bottom
