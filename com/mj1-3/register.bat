if "%1"=="register"     goto register
if "%1"=="unregister"   goto unregister
if "%1"=="pack"         goto pack
echo mjdll: usage: [pack register unregister]
goto exit
rem ---------------------------------------------------
:register
regsvr32 /c /s dtmj_grabit.ax
regsvr32 /c /s dtmj_encode.ax
regsvr32 /c /s dtmj_netout.ax
goto done
rem ---------------------------------------------------
:unregister
regsvr32 /u /s dtmj_grabit.ax
regsvr32 /u /s dtmj_encode.ax
regsvr32 /u /s dtmj_netout.ax
goto done
rem ---------------------------------------------------
:pack
regsvr32 /c /s dtmj_grabit.ax
regsvr32 /c /s dtmj_encode.ax
regsvr32 /c /s dtmj_netout.ax
goto done
rem ---------------------------------------------------
:done
echo mjdll: %1 done
goto exit
rem ---------------------------------------------------
:exit
