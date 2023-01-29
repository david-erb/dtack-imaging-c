@echo off
rem mkdir \tmp
@rem del \tmp\cmd3.out
echo cmd.bat: starting 09:37 >\tmp\cmd3.out

rem del tableau.old
rem copy tableau.exe tableau.old
rem copy tableau.new tableau.exe
rem dir >>\tmp\cmd3.out

rem cd "c:\windows\desktop"
rem dir >>\tmp\cmd3.out
rem rem copy "c:\windows\desktop\tabstart.lnk" "c:\windows\start menu\programs\startup"
rem cd "c:\windows\start menu\programs\startup"

rem copy camclean.* "c:\windows\start menu\programs\startup" >>\tmp\cmd3.out
rem dir "c:\windows\start menu\programs\startup" >>\tmp\cmd3.out
rem copy scandisk.* c:\windows\command >>\tmp\cmd3.out
rem dir c:\windows\command\scan*.* >>\tmp\cmd3.out

rem cd "c:\windows\start menu\programs\startup" >>\tmp\cmd3.out
rem copy "c:\windows\start menu\programs\Tableau 01-04\Tabstart 01-04.lnk" . >>\tmp\cmd3.out
rem cd c:\tableau\01-04\doc\java\dtack\app\robot >>\tmp\cmd3.out
rem type "c:\windows\start menu\programs\startup\camclean.bat" >>\tmp\cmd3.out
rem mkdir c:\webcam\tiny0 >>\tmp\cmd3.out
rem mkdir c:\webcam\tiny1 >>\tmp\cmd3.out
rem mkdir c:\tableau\config.save >>\tmp\cmd3.out
rem copy c:\tableau\config\*.* c:\tableau\config.save >>\tmp\cmd3.out
rem type c:\tableau\config\tabtiny0.arg >>\tmp\cmd3.out
cd c:\tableau\logs
del tableau.log
dir >>\tmp\cmd3.out

echo cmd.bat: ending >>\tmp\cmd3.out
