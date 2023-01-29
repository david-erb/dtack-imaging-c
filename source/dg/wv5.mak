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
statics: libs 

shared: shareds
shareds: shlibs 

allsubs:

exes: 

shexes: 

scripts: 

libs: \
	  $(OUT)\lib\dtdg.lib

shlibs: \
	  $(OUT)\dynamic\dtdg.dll

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\lib\dtdg.lib
	-del $(OUT)\dynamic\dtdg.dll
	-del $(OUT)\source\dg\*.obj
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




#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dtdg.lib: \
	  $(OUT)\source\dg\line.obj \
	  $(OUT)\source\dg\header.obj \
	  $(OUT)\source\dg\dgserver.obj \
	  $(OUT)\source\dg\request.obj \
	  $(OUT)\source\dg\response.obj \
	  $(OUT)\source\dg\rev.obj \
	  $(OUT)\source\dg\exit.obj \
	  $(OUT)\source\dg\delay.obj \
	  $(OUT)\source\dg\dgclient.obj \
	  $(OUT)\source\dg\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtdg.lib @<<
	  $(OUT)\source\dg\line.obj
	  $(OUT)\source\dg\header.obj
	  $(OUT)\source\dg\dgserver.obj
	  $(OUT)\source\dg\request.obj
	  $(OUT)\source\dg\response.obj
	  $(OUT)\source\dg\rev.obj
	  $(OUT)\source\dg\exit.obj
	  $(OUT)\source\dg\delay.obj
	  $(OUT)\source\dg\dgclient.obj
	  $(OUT)\source\dg\libmain.obj
<<
	
$(OUT)\dynamic\dtdg.dll \
$(OUT)\libdll\dtdg.lib: \
	  $(OUT)\source\dg\line.obj \
	  $(OUT)\source\dg\header.obj \
	  $(OUT)\source\dg\dgserver.obj \
	  $(OUT)\source\dg\request.obj \
	  $(OUT)\source\dg\response.obj \
	  $(OUT)\source\dg\rev.obj \
	  $(OUT)\source\dg\exit.obj \
	  $(OUT)\source\dg\delay.obj \
	  $(OUT)\source\dg\dgclient.obj \
	  $(OUT)\source\dg\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtdg.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtdg.lib
	  $(OUT)\source\dg\line.obj
	  $(OUT)\source\dg\header.obj
	  $(OUT)\source\dg\dgserver.obj
	  $(OUT)\source\dg\request.obj
	  $(OUT)\source\dg\response.obj
	  $(OUT)\source\dg\rev.obj
	  $(OUT)\source\dg\exit.obj
	  $(OUT)\source\dg\delay.obj
	  $(OUT)\source\dg\dgclient.obj
	  $(OUT)\source\dg\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\dg\line.obj: $(SRC)\source\dg\line.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\line.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\line.c
$(OUT)\source\dg\header.obj: $(SRC)\source\dg\header.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\header.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\header.c
$(OUT)\source\dg\dgserver.obj: $(SRC)\source\dg\dgserver.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\dgserver.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\dgserver.c
$(OUT)\source\dg\request.obj: $(SRC)\source\dg\request.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\request.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\request.c
$(OUT)\source\dg\response.obj: $(SRC)\source\dg\response.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\response.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\response.c
$(OUT)\source\dg\rev.obj: $(SRC)\source\dg\rev.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\rev.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\rev.c
$(OUT)\source\dg\exit.obj: $(SRC)\source\dg\exit.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\exit.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\exit.c
$(OUT)\source\dg\delay.obj: $(SRC)\source\dg\delay.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\delay.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\delay.c
$(OUT)\source\dg\dgclient.obj: $(SRC)\source\dg\dgclient.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\dgclient.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\dgclient.c
$(OUT)\source\dg\libmain.obj: $(SRC)\source\dg\libmain.c \
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
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\dg\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\dg \
	  -I$(SRC)\include \
	  $(SRC)\source\dg\libmain.c


