@echo off
if "%1"=="upload" goto upload
if "%1"=="dynamic" goto dynamic
if "%1"=="dynamic_upload" goto dynamic_upload
if "%1"=="static" goto static
if "%1"=="static_upload" goto static_upload

%DT_MAKE% %DT_SRC%\%1\%DT_TARGET%.mak SRC=%DT_SRC% OUT=%DT_OUT% %2
@echo off
if "%3"=="x" goto combine_makeout
@rem more  smake.out
@goto done
:combine_makeout
type smake.out >> make.out
@goto done
rem =================================================
:upload

@type %DT_FTPIN%                            >  %DT_OUT%\tmp\tmp.ftp
@echo cd %DT_UPLOAD%\%DT_TARGET%\%2         >> %DT_OUT%\tmp\tmp.ftp
@echo lcd %2                                >> %DT_OUT%\tmp\tmp.ftp
@echo mput %3                               >> %DT_OUT%\tmp\tmp.ftp
@echo lcd ..                                >> %DT_OUT%\tmp\tmp.ftp
@type %DT_FTPOUT%                           >> %DT_OUT%\tmp\tmp.ftp

@call myftp %DT_OUT%\tmp\tmp.ftp
@goto done
rem =================================================
:dynamic_upload

@type %DT_FTPIN%                            >  %DT_OUT%\tmp\tmp.ftp
@echo mkdir %DT_UPLOAD%                     >> %DT_OUT%\tmp\tmp.ftp
@echo mkdir %DT_UPLOAD%\%DT_TARGET%         >> %DT_OUT%\tmp\tmp.ftp
@echo mkdir %DT_UPLOAD%\%DT_TARGET%\dynamic >> %DT_OUT%\tmp\tmp.ftp
@echo cd %DT_UPLOAD%\%DT_TARGET%\dynamic    >> %DT_OUT%\tmp\tmp.ftp
@echo lcd dynamic                           >> %DT_OUT%\tmp\tmp.ftp
@echo mput *.dll                            >> %DT_OUT%\tmp\tmp.ftp
@echo mput tableau.exe                      >> %DT_OUT%\tmp\tmp.ftp
@echo lcd ..                                >> %DT_OUT%\tmp\tmp.ftp
@type %DT_FTPOUT%                           >> %DT_OUT%\tmp\tmp.ftp

@call myftp %DT_OUT%\tmp\tmp.ftp
@goto done
rem =================================================
:dynamic
dtutil del make.out
call %DT_SRC%\bat\b.bat fourth shlibs x
call %DT_SRC%\bat\b.bat source shlibs x
call %DT_SRC%\bat\b.bat app\tableau shared x
call %DT_SRC%\bat\b.bat upload dynamic *.dll
call %DT_SRC%\bat\b.bat upload dynamic *.exe

@goto done
rem =================================================
:static_upload

@type %DT_FTPIN%                            >  %DT_OUT%\tmp\tmp.ftp
@echo mkdir %DT_UPLOAD%                     >> %DT_OUT%\tmp\tmp.ftp
@echo mkdir %DT_UPLOAD%\%DT_TARGET%         >> %DT_OUT%\tmp\tmp.ftp
@echo mkdir %DT_UPLOAD%\%DT_TARGET%\static  >> %DT_OUT%\tmp\tmp.ftp
@echo cd %DT_UPLOAD%\%DT_TARGET%\static     >> %DT_OUT%\tmp\tmp.ftp
@echo lcd static                            >> %DT_OUT%\tmp\tmp.ftp
@echo mput tableau.exe                      >> %DT_OUT%\tmp\tmp.ftp
@echo lcd ..                                >> %DT_OUT%\tmp\tmp.ftp
@type %DT_FTPOUT%                           >> %DT_OUT%\tmp\tmp.ftp

@call myftp %DT_OUT%\tmp\tmp.ftp
@goto done
rem =================================================
:static
dtutil del make.out
@rem call %DT_SRC%\bat\b.bat fourth libs x
call %DT_SRC%\bat\b.bat source libs x
call %DT_SRC%\bat\b.bat app\tableau static x
call %DT_SRC%\bat\b.bat static_upload

@goto done
rem =================================================
:done
