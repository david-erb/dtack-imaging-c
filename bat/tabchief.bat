@echo on
if "%1"=="test" goto test
:server
static\tabchief 0 0 0 -conn 0:5780 -conf c:\tableau\config -errto c:\tableau\logs\tabchief.log -dbgto c:\tableau\logs\tabchief.log -sayto c:\tableau\logs\tabchief.log -D usr1+usr2+ipc
goto exit

@rem -----------------------------------------------------------------------
@rem make sure /tmp exists on both local and remote
@rem use -reps for a long-running test

:test
del test.log
static\tabchief 0 0 0 -op test -conn 192.9.200.4:5780 -conf /tmp -input test.tmp -output /tmp/test.tmp -errto test.log -dbgto test.log -sayto test.log -reps 1 -D ""
goto exit

:exit
@echo tabchief.bat: exiting
