@echo off
rem # Must be run from source directory.
rem # Arguments are:
rem # 1.   simple name of program, same as what program puts as prefix in all
rem #      messages it prints
rem # 2.   DTROOT directory with subdirectory named "static" where executable is
rem #      and which has writable subdirectory named "tmp" where temporary files
rem #      can be written
rem # 3-9. actual program invocation

if "%1"=="" goto badarg1
if "%2"=="" goto badarg2
if "%3"=="" goto badarg3

dtutil time %1: being started by dttest1.bat _date_ _time_
dtutil time %1: being started by dttest1.bat _date_ _time_>> %2\test.out

rem # Make a batch file in tmp.bat which does one of two things:
rem #   1. execute the named program if the executable exists
rem #   2. print a message about the executable not existing
rem #      The format of this message is such that "dtutil endcheck"
rem #      considers it a valid program termination.
rem #      The philosophy here is that some other program can verify
rem #      and report errors if expected files are not present.
rem # The whole idea here is that a non-existent executable is noted in
rem # test.out, but not reported as an error by "dtutil endcheck".

dtutil isexecutable %2\static\%3.exe %1 %2\tmp\tmp.out > %2\tmp\tmp.bat

rem # Remove old tmp.out, if any.
dtutil del %2\tmp\tmp.out

rem # Call tmp.bat batch file produced above.
rem type %2\tmp\tmp.bat
call %2\tmp\tmp.bat %4 %5 %6 %7 %8 %9

rem # Concatenate output from program run to master test.out file.
type %2\tmp\tmp.out >> %2\test.out

goto bottom
rem =====================================================
:badarg1
echo dttest.bat: missing test name argument
goto bottom
rem =====================================================
:badarg2
echo dttest.bat: missing OUT directory name argument
goto bottom
rem =====================================================
:badarg 3
echo dttest.bat: missing executable name argument
goto bottom

:bottom
