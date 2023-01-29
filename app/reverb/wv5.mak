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
	  $(OUT)\static\reverb.exe

shexes: \
	  $(OUT)\dynamic\reverb.exe

scripts: 

libs: \
	  $(OUT)\lib\reverbx.lib

shlibs: \
	  $(OUT)\dynamic\reverbx.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\reverb.exe
	-del $(OUT)\static\reverb.exe
	-del $(OUT)\lib\reverbx.lib
	-del $(OUT)\dynamic\reverbx.dll
	-del $(OUT)\app\reverb\*.obj
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



$(OUT)\static\reverb.exe: \
	  $(OUT)\app\reverb\reverb.obj \
	  $(OUT)\lib\reverbx.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\reverb.exe @<<
	  /MAP:$(OUT)\static\reverb.map
	  /PDB:$(OUT)\static\reverb.pdb
 /DEBUG /STACK:64000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\reverb\reverb.obj
	$(OUT)\lib\reverbx.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\reverb.res
<<
$(OUT)\dynamic\reverb.exe: \
	  $(OUT)\app\reverb\reverb.obj \
	  $(OUT)\libdll\reverbx.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\reverb.exe @<<
	  /MAP:$(OUT)\dynamic\reverb.map 
	  /PDB:$(OUT)\dynamic\reverb.pdb 

	  /DEBUG
	  /STACK:64000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\reverb\reverb.obj
	  $(OUT)\libdll\reverbx.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\reverb.lib
	del $(OUT)\dynamic\reverb.exp





#-------------------------------------------------------------------------
# libs



$(OUT)\lib\reverbx.lib: \
	  $(OUT)\app\reverb\client.obj \
	  $(OUT)\app\reverb\libmain.obj
	lib /nologo /out:$(OUT)\lib\reverbx.lib @<<
	  $(OUT)\app\reverb\client.obj
	  $(OUT)\app\reverb\libmain.obj
<<
	
$(OUT)\dynamic\reverbx.dll \
$(OUT)\libdll\reverbx.lib: \
	  $(OUT)\app\reverb\client.obj \
	  $(OUT)\app\reverb\libmain.obj \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\reverbx.dll @<<!
	  /dll /implib:$(OUT)\libdll\reverbx.lib
	  $(OUT)\app\reverb\client.obj
	  $(OUT)\app\reverb\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\app\reverb\client.obj: $(SRC)\app\reverb\client.c \
	  $(SRC)\app\reverb\reverbi.h \
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
	  $(SRC)\include\reverb.h \
	  $(SRC)\include\reverb.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\reverb\client.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\reverb \
	  -I$(SRC)\include \
	  $(SRC)\app\reverb\client.c
$(OUT)\app\reverb\libmain.obj: $(SRC)\app\reverb\libmain.c \
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
	   /Fo$(OUT)\app\reverb\libmain.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\reverb \
	  -I$(SRC)\include \
	  $(SRC)\app\reverb\libmain.c
$(OUT)\app\reverb\reverb.obj: $(SRC)\app\reverb\reverb.c \
	  $(SRC)\app\reverb\reverbi.h \
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
	  $(SRC)\include\reverb.h \
	  $(SRC)\include\reverb.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\app\reverb\reverb.use
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\reverb\reverb.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\reverb \
	  -I$(SRC)\include \
	  $(SRC)\app\reverb\reverb.c


