@echo off
if "%1"=="loop"       goto ok
if "%1"=="once"       goto ok
echo pump.bat: arg must be "loop" or "once"
goto exit

:ok
@rem those with no defaults can go in evironment variables
set DT_ERRTO=out
set DT_DBGTO=out
set DT_SAYTO=out
set DT_INPUT=roll
set DT_D=usr1+usr2+ipc+complex

:loop
rem static\tableau 0 0 0 -path c: -in dir,d:\tmp\gary2.raw\%03d -scr socket,192.9.200.4:5711+tmp.x -var jpeg -t 40 -color rgb888
static\tableau 0 0 0 

if "%1"=="once" goto once

dtutil tail out `
goto loop

:once
type out

:exit
echo pump.bat: exiting
