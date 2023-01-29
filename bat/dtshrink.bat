@echo off
if "%1"=="data"     goto _data
if "%1"=="img"      goto _img
if "%1"=="isvb11"   goto _isvb11
if "%1"=="lead6016" goto _lead6016
if "%1"=="lead6032" goto _lead6032
if "%1"=="wb3s"     goto _wb3s
if "%1"=="wb3dll"   goto _wb3dll
if "%1"=="wb3d"     goto _wb3d
if "%1"=="bc45"     goto _bc45
if "%1"=="vb416"  goto _vb416
if "%1"=="vb432"      goto _vb432
if "%1"=="dtack"    goto _dtack
if "%1"=="tableau"  goto _tableau
if "%1"=="upload"   goto _upload
echo dtshrink: invalid argument
goto _exit
rem --------------------------------------------------
:_data
k:
cd data
c:\sw\pcinst40\shrink d:\distrib\data.shr *.* +s
cd ..
d:
echo .
goto _exit
rem --------------------------------------------------
:_img
k:
cd ..\dtdist\img
c:\sw\pcinst40\shrink d:\distrib\img.shr *.* +s
cd ..\..\dt
d:
echo .
goto _exit
rem --------------------------------------------------
:_isvb11
k:
cd ..\dtdist\isvb11
c:\sw\pcinst40\shrink d:\distrib\isvb11.shr *.* +s
cd ..\..\dt
d:
echo .
goto _exit
rem --------------------------------------------------
:_lead6016
c:\sw\pcinst40\shrink d:\distrib\lead6016.shr @k:\prj\dt\pcinst\lead6016.lst
echo .
goto _exit
rem --------------------------------------------------
:_lead6032
c:\sw\pcinst40\shrink d:\distrib\lead6032.shr @k:\prj\dt\pcinst\lead6032.lst
echo .
goto _exit
rem --------------------------------------------------
:_wb3s
c:\sw\pcinst40\shrink d:\distrib\wb3s.shr d:\prj\dtwb3\static\*.exe
echo .
goto _exit
rem --------------------------------------------------
:_wb3dll
c:\sw\pcinst40\shrink d:\distrib\wb3dll.shr d:\prj\dtwb3\dynamic\*.dll
echo .
goto _exit
rem --------------------------------------------------
:_wb3d
c:\sw\pcinst40\shrink d:\distrib\wb3d.shr d:\prj\dtwb3\dynamic\*.exe
echo .
goto _exit
rem --------------------------------------------------
:_bc45
c:\sw\pcinst40\shrink d:\distrib\bc45.shr k:\prj\dtdist\bc45\*.*
echo .
goto _exit
rem --------------------------------------------------
:_vb416
c:\sw\pcinst40\shrink d:\distrib\vb416.shr k:\prj\dtdist\vb416\*.*
echo .
goto _exit
rem --------------------------------------------------
:_vb432
c:\sw\pcinst40\shrink k:\prj\dtdist\shr\vb432.shr k:\prj\dtdist\vb432\*.*
echo .
goto _exit
rem --------------------------------------------------
:_dtack
c:\sw\pcinst40\shrink d:\distrib\dtack.shr k:\prj\dt\vb40\dtack.ini
echo .
goto _exit
rem --------------------------------------------------
:_tableau
c:\swnt\sw\pcinst40\shrink k:\prj\dtdist\shr\tableau.shr k:\prj\dtdist\tableau\*.*
echo .
goto _exit
rem --------------------------------------------------
:_upload

type c:\ftp\dlogin.ftp                     >  c:\tmp\tmp.ftp
echo cd /d/2/prj/dtdist/shr                >> c:\tmp\tmp.ftp
echo lcd d:\distrib                        >> c:\tmp\tmp.ftp
echo mput %2                               >> c:\tmp\tmp.ftp
type c:\ftp\logout.ftp                     >> c:\tmp\tmp.ftp

call myftp c:\tmp\tmp.ftp
goto _exit
rem --------------------------------------------------
:_exit


