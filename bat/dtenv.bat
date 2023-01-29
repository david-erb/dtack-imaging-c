@echo off
rem set up environment variables given environment string as argument
rem if unknown env, sets DT_BATCHERR, else unsets it
set DT_BATCHERR=
rem
rem The following environment variables tell the makefile where to find
rem the external packages.
rem Please modify them according to your computer's setup.
rem Please note that not all of the variables are used in all makefiles.
rem
rem =================================================
@set DT_DRIVE=d:
if "%2"=="" goto nodisk
@set DT_DRIVE=%2
:nodisk
rem =================================================
if "%1"=="bc45w32_unset"    goto bc45w32_unset
if "%1"=="bc45dos_unset"    goto bc45dos_unset
if "%1"=="bc31_unset"       goto bc31_unset
if "%1"=="tc30_unset"       goto tc30_unset
if "%1"=="msvc50_unset"     goto msvc50_unset
if "%1"=="msvc40_unset"     goto msvc40_unset
if "%1"=="msvc15_unset"     goto msvc15_unset
if "%1"=="wat10a_unset"     goto wat10a_unset
if "%1"=="bc45w32"          goto global_set
if "%1"=="bc45dos"          goto global_set
if "%1"=="bc31"             goto global_set
if "%1"=="tc30"             goto global_set
if "%1"=="msvc50"           goto global_set
if "%1"=="msvc40"           goto global_set
if "%1"=="msvc15"           goto global_set
if "%1"=="wat10a"           goto global_set
echo dtenv: unknown environment "%1"
set DT_BATCHERR=1
goto done

rem =================================================
rem save old values of important environment variables maybe used by others
:global_set
set DT_PATH_OLD=%PATH%
set DT_COMPILER_OLD=%COMPILER%
set DT_INCLUDE_OLD=%INCLUDE%
set DT_LIB_OLD=%LIB%

rem =================================================
if "%1"=="bc45dos"       goto bc45dos
if "%1"=="bc45w32"    goto bc45w32
if "%1"=="bc31"       goto bc31
if "%1"=="tc30"       goto tc30
if "%1"=="msvc50"     goto msvc50
if "%1"=="msvc40"     goto msvc40
if "%1"=="msvc15"     goto msvc15
if "%1"=="wat10a"     goto wat10a
echo dtenv: unknown environment "%1"
set DT_BATCHERR=1
goto done

rem =================================================
rem Borland C++ Version 3.1
:bc31
set DT_MEMCHECK=%DT_DRIVE%\sw\mc30\borlandc
set COMPILER=%DT_DRIVE%\sw\borlandc\bin
set INCLUDE=
set LIB=
set DT_MAKE=maker /i /f
PATH %PATH%;%COMPILER%
goto common_dos_set
:bc31_unset
set DT_MEMCHECK=
goto common_unset

rem =================================================
rem Borland C++ Version 4.5
:bc45dos
set DT_MEMCHECK=%DT_DRIVE%\sw\mc30\borlandc
set COMPILER=%DT_DRIVE%\bc45\bin
set INCLUDE=
set LIB=
set DT_MAKE=maker /i /f
PATH %PATH%;%COMPILER%
goto common_dos_set
:bc45dos_unset
set DT_MEMCHECK=
goto common_unset

rem =================================================
rem Borland C++ Version 4.5 for win32
:bc45w32
set DT_MEMCHECK=%DT_DRIVE%\sw\mc30\borlandc
set COMPILER=%DT_DRIVE%\bc45\bin
set INCLUDE=
set LIB=
set DT_MAKE=maker /i /f
PATH %PATH%;%COMPILER%
goto common_w32_set
:bc45w32_unset
set DT_MEMCHECK=
goto common_unset

rem =================================================
rem Borland Turbo C++ Version 3.0
:tc30
set COMPILER=%DT_DRIVE%\sw\tc30\bin
set INCLUDE=
set LIB=
set DT_MAKE=maker /i /f
PATH %PATH%;%COMPILER%
goto common_dos_set
:tc30_unset
goto common_unset

rem =================================================
rem Microsoft Visual C++ Version 5.0
:msvc50
set COMPILER=%DT_DRIVE%\msvc50\DevStudio\VC\bin
set INCLUDE=%DT_DRIVE%\msvc50\DevStudio\VC\include;%DT_DRIVE%\msvc50\DevStudio\VC\mfc\include
set LIB=%DT_DRIVE%\msvc50\DevStudio\VC\lib;%DT_DRIVE%\msvc50\DevStudio\VC\mfc\lib
set DT_ROGUE=%DT_DRIVE%\rw-95-msvc5
set DT_MAKE=nmake /nologo /i /x- /f
PATH %PATH%;%COMPILER%;%DT_DRIVE%\msvc50\DevStudio\SharedIDE\bin
goto common_w32_set
:msvc50_unset
set DT_MEMCHECK=
goto common_unset

rem =================================================
rem Microsoft Visual C++ Version 4.0
:msvc40
set COMPILER=%DT_DRIVE%\msvc40\bin
set INCLUDE=%DT_DRIVE%\msvc40\include
set LIB=%DT_DRIVE%\msvc40\lib
set DT_MAKE=nmake /nologo /i /x- /f
PATH %PATH%;%COMPILER%
goto common_w32_set
:msvc40_unset
set DT_MEMCHECK=
goto common_unset

rem =================================================
rem Microsoft Visual C++ Version 1.5
:msvc15
set DT_MEMCHECK=%DT_DRIVE%\sw\mc30\msvc15
set COMPILER=%DT_DRIVE%\msvc15\bin
set INCLUDE=%DT_DRIVE%\msvc15\include
set LIB=%DT_DRIVE%\msvc15\lib
set DT_MAKE=nmake /nologo /i /x- /f
PATH %PATH%;%COMPILER%
goto common_w16_set
:msvc15_unset
set DT_MEMCHECK=
goto common_unset

rem =================================================
rem Watcom C/C++ Version 10.0A
:wat10a
set DT_MEMCHECK=%DT_DRIVE%\sw\mc30\wat10a
set DT_EDPATH_OLD=%EDPATH%
set DT_WATCOM_OLD=%WATCOM%
set WATCOM=%DT_DRIVE%\sw\wat10a
set EDPATH=%WATCOM%\eddat
set COMPILER=%WATCOM%\bin;%WATCOM%\binb
set INCLUDE=%WATCOM%\h;%WATCOM%\h\win
set LIB=
if "%DT_MEMCHECK%"=="" goto norocket
set LIB=%DT_MEMCHECK%\lib
:norocket
set DT_MAKE=wmake /k /f
PATH %PATH%;%COMPILER%
goto common_dos_set
:wat10a_unset
set EDPATH=%DT_EDPATH_OLD%
set WATCOM=%DT_WATCOM_OLD%
set DT_EDPATH_OLD=
set DT_WATCOM_OLD=
set DT_MEMCHECK=
goto common_unset

rem =================================================
:common_dos_set
rem set environment variables used only by Dtack development scripts
rem
set DT_EMSIF=%DT_DRIVE%\local\lib\emsif24a
set DT_XMSIF=%DT_DRIVE%\local\lib\xmsif15
set DT_XMSLB=%DT_DRIVE%\local\lib\xmslb17
set DT_TXMS=%DT_DRIVE%\local\lib\txms10
set DT_FASTGRAPH3=%DT_DRIVE%\local\lib\fg305
set DT_FASTGRAPH4=%DT_DRIVE%\local\lib\fg401
set DT_FASTGRAPH_CARD=ati,800
set DT_FGW=%DT_DRIVE%\local\lib\fgw102
rem
rem     SVGACC is a shareware library for super VGA support.
rem     Used within Dtack software for image display only.
rem     Available on SimTel in msdos/graphics.
rem     Makefile expects two files in this directory: svgacc.h and svgacc.lib.
set DT_SVGACC=%DT_DRIVE%\local\lib\svgacc23
set DT_SVGACC_CARD=auto,800
goto done

rem =================================================
:common_w32_set
rem set environment variables used only by Dtack development scripts
rem
set DT_WINNOV_DEF=DT_1
set DT_WINNOV_DIR=%DT_DRIVE%\local\lib\winnov
set DT_FGW=%DT_DRIVE%\local\lib\fgw102
goto done

rem =================================================
:common_w16_set
rem set environment variables used only by Dtack development scripts
rem
set DT_FGW=%DT_DRIVE%\local\lib\fgw102
goto done

rem =================================================
:common_unset
set DT_EMSIF=
set DT_TXMS=
set DT_XMSLB=
set DT_XMSIF=
set DT_WINNOV=
set DT_FGW=
set DT_FASTGRAPH_CARD=
set DT_FASTGRAPH4=
set DT_FASTGRAPH3=
set DT_SVGACC_CARD=
set DT_SVGACC=
set DT_MAKE=
PATH %DT_PATH_OLD%
set COMPILER=%DT_COMPILER_OLD%
set INCLUDE=%DT_INCLUDE_OLD%
set LIB=%DT_LIB_OLD%
set DT_LIB_OLD=
set DT_INCLUDE_OLD=
set DT_COMPILER_OLD=
set DT_PATH_OLD=
goto done

rem =================================================
set DT_DRIVE=
:done
