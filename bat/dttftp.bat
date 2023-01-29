if "%1"=="inter" goto inter
del tmp.out
dttftp 0 0 0 -errto tmp.out -dbgto tmp.out -D command rocket,192.9.200.6:21 5700 pcftp page.
goto exit
:inter
dttftp 0 1 0 -D command rocket,192.9.200.6:21 5700 pcftp page.
:exit