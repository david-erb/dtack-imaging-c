@echo on
@rem
@rem This file runs the compiler.
@rem It presumes you are in the top level directory of the component.
@rem With 1 argument, this file makes everything in the component.
@rem With 2 arguments, it makes only a single subdirectory.
@rem WIth 3 argument, it makes only one target in a single subdirectory.
@rem
@if "%1"=="" goto arg0
@if "%2"=="" goto arg1
@if "%4"=="" goto arg2
nmake /nologo /f%DT_BASE_SRC%\%2\%1.mak SET=%DT_SET% SRC=%DT_BASE_SRC% OUT=%DT_BASE_OUT% %3 >%4
@goto exit
:arg2
nmake /nologo /f%DT_BASE_SRC%\%2\%1.mak SET=%DT_SET% SRC=%DT_BASE_SRC% OUT=%DT_BASE_OUT% %3
@goto exit
:arg1
nmake /nologo /f%DT_BASE_SRC%\%1.mak SET=%DT_SET% SRC=%DT_BASE_SRC% OUT=%DT_BASE_OUT% statics
:arg0
@echo dtmake.bat: usage: system subdir target
:exit
