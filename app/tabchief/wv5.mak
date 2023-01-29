# This makefile is input to nmake, the Microsoft make utility.
# This makefile assumes the following:
# 1. that the INCLUDE environment variable points to the standard Microsoft
#    include directories, for example:
#      set INCLUDE=c:\msvc\include
# 2. that the PATH environment variable has the compiler in it, for example:
#      PATH=c:\mvsc\bin;c:\dos

!include "$(SET)\wv5.set"

#                                   optimize for speed
OPTIMIZE_HIGH = /O2

#                                   good optimization but still able to debug
OPTIMIZE_LOW = 




default: shared


all: statics shareds others


static: statics
statics: libs exes 

shared: shareds
shareds: shlibs shexes 

allsubs:

exes: \
	  $(OUT)\static\tabchief.exe

shexes: \
	  $(OUT)\dynamic\tabchief.exe

scripts: 

libs: \
	  $(OUT)\lib\dschedx.lib \
	  $(OUT)\lib\tabchiefx.lib

shlibs: \
	  $(OUT)\dynamic\dschedx.dll \
	  $(OUT)\dynamic\tabchiefx.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\tabchief.exe
	-del $(OUT)\static\tabchief.exe
	-del $(OUT)\lib\dschedx.lib
	-del $(OUT)\lib\tabchiefx.lib
	-del $(OUT)\dynamic\dschedx.dll
	-del $(OUT)\dynamic\tabchiefx.dll
	-del $(OUT)\app\tabchief\*.obj
	-del *.out
	-del tmp
	-del *.map

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)\static\tabchief.exe: \
	  $(OUT)\app\tabchief\tabchief.obj \
	  $(OUT)\lib\tabchiefx.lib \
	  $(OUT)\lib\dschedx.lib \
	  $(OUT)\lib\reverbx.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\tabchief.exe @<<
	  /MAP:$(OUT)\static\tabchief.map
	  /PDB:$(OUT)\static\tabchief.pdb
 /DEBUG /STACK:64000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\tabchief\tabchief.obj
	$(OUT)\lib\tabchiefx.lib
	$(OUT)\lib\dschedx.lib
	$(OUT)\lib\reverbx.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\tabchief.res
<<
$(OUT)\dynamic\tabchief.exe: \
	  $(OUT)\app\tabchief\tabchief.obj \
	  $(OUT)\libdll\tabchiefx.lib \
	  $(OUT)\libdll\dschedx.lib \
	  $(OUT)\libdll\reverbx.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\tabchief.exe @<<
	  /MAP:$(OUT)\dynamic\tabchief.map 
	  /PDB:$(OUT)\dynamic\tabchief.pdb 

	  /DEBUG
	  /STACK:64000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\tabchief\tabchief.obj
	  $(OUT)\libdll\tabchiefx.lib
	  $(OUT)\libdll\dschedx.lib
	  $(OUT)\libdll\reverbx.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\tabchief.lib
	del $(OUT)\dynamic\tabchief.exp



#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dschedx.lib: \
	  $(OUT)\app\tabchief\dschedl.obj \
	  $(OUT)\app\tabchief\libmain.obj \
	  $(OUT)\app\tabchief\dsch_da.obj
	lib /nologo /out:$(OUT)\lib\dschedx.lib @<<
	  $(OUT)\app\tabchief\dschedl.obj
	  $(OUT)\app\tabchief\libmain.obj
	  $(OUT)\app\tabchief\dsch_da.obj
<<
	
$(OUT)\dynamic\dschedx.dll \
$(OUT)\libdll\dschedx.lib: \
	  $(OUT)\app\tabchief\dschedl.obj \
	  $(OUT)\app\tabchief\libmain.obj \
	  $(OUT)\app\tabchief\dsch_da.obj \
	  $(OUT)\app\tabchief\dsch_da.obj \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\dschedx.dll @<<!
	  /dll /implib:$(OUT)\libdll\dschedx.lib
	  $(OUT)\app\tabchief\dschedl.obj
	  $(OUT)\app\tabchief\libmain.obj
	  $(OUT)\app\tabchief\dsch_da.obj
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	lib /nologo /out:$(OUT)\libdll\dschedx.lib @<<!
	  $(OUT)\libdll\dschedx.lib
	  $(OUT)\app\tabchief\dsch_da.obj
<<
$(OUT)\lib\tabchiefx.lib: \
	  $(OUT)\app\tabchief\dsched.obj \
	  $(OUT)\app\tabchief\client.obj \
	  $(OUT)\app\tabchief\copy.obj \
	  $(OUT)\app\tabchief\edit.obj \
	  $(OUT)\app\tabchief\file.obj \
	  $(OUT)\app\tabchief\index.obj \
	  $(OUT)\app\tabchief\reply.obj \
	  $(OUT)\app\tabchief\restart.obj \
	  $(OUT)\app\tabchief\save.obj \
	  $(OUT)\app\tabchief\status.obj \
	  $(OUT)\app\tabchief\server.obj \
	  $(OUT)\app\tabchief\split.obj \
	  $(OUT)\app\tabchief\testcopy.obj \
	  $(OUT)\app\tabchief\testqs.obj \
	  $(OUT)\app\tabchief\libmain.obj
	lib /nologo /out:$(OUT)\lib\tabchiefx.lib @<<
	  $(OUT)\app\tabchief\dsched.obj
	  $(OUT)\app\tabchief\client.obj
	  $(OUT)\app\tabchief\copy.obj
	  $(OUT)\app\tabchief\edit.obj
	  $(OUT)\app\tabchief\file.obj
	  $(OUT)\app\tabchief\index.obj
	  $(OUT)\app\tabchief\reply.obj
	  $(OUT)\app\tabchief\restart.obj
	  $(OUT)\app\tabchief\save.obj
	  $(OUT)\app\tabchief\status.obj
	  $(OUT)\app\tabchief\server.obj
	  $(OUT)\app\tabchief\split.obj
	  $(OUT)\app\tabchief\testcopy.obj
	  $(OUT)\app\tabchief\testqs.obj
	  $(OUT)\app\tabchief\libmain.obj
<<
	
$(OUT)\dynamic\tabchiefx.dll \
$(OUT)\libdll\tabchiefx.lib: \
	  $(OUT)\app\tabchief\dsched.obj \
	  $(OUT)\app\tabchief\client.obj \
	  $(OUT)\app\tabchief\copy.obj \
	  $(OUT)\app\tabchief\edit.obj \
	  $(OUT)\app\tabchief\file.obj \
	  $(OUT)\app\tabchief\index.obj \
	  $(OUT)\app\tabchief\reply.obj \
	  $(OUT)\app\tabchief\restart.obj \
	  $(OUT)\app\tabchief\save.obj \
	  $(OUT)\app\tabchief\status.obj \
	  $(OUT)\app\tabchief\server.obj \
	  $(OUT)\app\tabchief\split.obj \
	  $(OUT)\app\tabchief\testcopy.obj \
	  $(OUT)\app\tabchief\testqs.obj \
	  $(OUT)\app\tabchief\libmain.obj \
	  $(OUT)\libdll\dschedx.lib \
	  $(OUT)\libdll\reverbx.lib \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\tabchiefx.dll @<<!
	  /dll /implib:$(OUT)\libdll\tabchiefx.lib
	  $(OUT)\app\tabchief\dsched.obj
	  $(OUT)\app\tabchief\client.obj
	  $(OUT)\app\tabchief\copy.obj
	  $(OUT)\app\tabchief\edit.obj
	  $(OUT)\app\tabchief\file.obj
	  $(OUT)\app\tabchief\index.obj
	  $(OUT)\app\tabchief\reply.obj
	  $(OUT)\app\tabchief\restart.obj
	  $(OUT)\app\tabchief\save.obj
	  $(OUT)\app\tabchief\status.obj
	  $(OUT)\app\tabchief\server.obj
	  $(OUT)\app\tabchief\split.obj
	  $(OUT)\app\tabchief\testcopy.obj
	  $(OUT)\app\tabchief\testqs.obj
	  $(OUT)\app\tabchief\libmain.obj
	  $(OUT)\libdll\dschedx.lib
	  $(OUT)\libdll\reverbx.lib
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\app\tabchief\dsch_da.obj: $(SRC)\app\tabchief\dsch_da.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\dsch_da.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\dsch_da.c
$(OUT)\app\tabchief\dschedl.obj: $(SRC)\app\tabchief\dschedl.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\dschedl.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\dschedl.c
$(OUT)\app\tabchief\libmain.obj: $(SRC)\app\tabchief\libmain.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\libmain.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\libmain.c
$(OUT)\app\tabchief\dsched.obj: $(SRC)\app\tabchief\dsched.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\dsched.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\dsched.c
$(OUT)\app\tabchief\client.obj: $(SRC)\app\tabchief\client.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\client.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\client.c
$(OUT)\app\tabchief\copy.obj: $(SRC)\app\tabchief\copy.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\copy.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\copy.c
$(OUT)\app\tabchief\edit.obj: $(SRC)\app\tabchief\edit.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\edit.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\edit.c
$(OUT)\app\tabchief\file.obj: $(SRC)\app\tabchief\file.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\file.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\file.c
$(OUT)\app\tabchief\index.obj: $(SRC)\app\tabchief\index.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\index.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\index.c
$(OUT)\app\tabchief\reply.obj: $(SRC)\app\tabchief\reply.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\reply.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\reply.c
$(OUT)\app\tabchief\restart.obj: $(SRC)\app\tabchief\restart.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\restart.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\restart.c
$(OUT)\app\tabchief\save.obj: $(SRC)\app\tabchief\save.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\save.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\save.c
$(OUT)\app\tabchief\status.obj: $(SRC)\app\tabchief\status.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\status.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\status.c
$(OUT)\app\tabchief\server.obj: $(SRC)\app\tabchief\server.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p \
	  $(SRC)\include\reverb.h \
	  $(SRC)\include\reverb.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\server.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\server.c
$(OUT)\app\tabchief\split.obj: $(SRC)\app\tabchief\split.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\split.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\split.c
$(OUT)\app\tabchief\testcopy.obj: $(SRC)\app\tabchief\testcopy.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\testcopy.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\testcopy.c
$(OUT)\app\tabchief\testqs.obj: $(SRC)\app\tabchief\testqs.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\testqs.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\testqs.c
$(OUT)\app\tabchief\tabchief.obj: $(SRC)\app\tabchief\tabchief.c \
	  $(SRC)\app\tabchief\tabchief.h \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\app\tabchief\dsched.h \
	  $(SRC)\app\tabchief\dsched.p \
	  $(SRC)\app\tabchief\tabchief.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tabchief\tabchief.use
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabchief\tabchief.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabchief \
	  -I$(SRC)\include \
	  $(SRC)\app\tabchief\tabchief.c


