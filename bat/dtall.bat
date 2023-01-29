@echo off
call dtset

echo ========================================= >  error.out 
echo dtall:                                    >> error.out 
echo ========================================= >  warning.out 
echo dtall:                                    >> warning.out 
echo ========================================= >  fail.out 
echo dtall:                                    >> fail.out 
echo ========================================= >  timing.out 
echo dtall:                                    >> timing.out 

call dtunpack dv0 inst

call dtunpack db0 inst

call dtunpack dt0 inst

call dtunpack dw0 inst

call dtunpack dw1 inst

call dtunpack wb0 inst

call dtunpack ww0 inst

:exit
