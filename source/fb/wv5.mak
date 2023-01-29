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
	  $(OUT)\lib\dtfb.lib

shlibs: \
	  $(OUT)\dynamic\dtfb.dll

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\lib\dtfb.lib
	-del $(OUT)\dynamic\dtfb.dll
	-del $(OUT)\source\fb\*.obj
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



$(OUT)\lib\dtfb.lib: \
	  $(OUT)\source\fb\fb.obj \
	  $(OUT)\source\fb\fbvidmux.obj \
	  $(OUT)\source\fb\fbcortex.obj \
	  $(OUT)\source\fb\fbdir.obj \
	  $(OUT)\source\fb\fbgen.obj \
	  $(OUT)\source\fb\fbmeteor.obj \
	  $(OUT)\source\fb\fbbt.obj \
	  $(OUT)\source\fb\fbvbmci.obj \
	  $(OUT)\source\fb\fbvfw.obj \
	  $(OUT)\source\fb\fbsnap.obj \
	  $(OUT)\source\fb\fbutil.obj \
	  $(OUT)\source\fb\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtfb.lib @<<
	  $(OUT)\source\fb\fb.obj
	  $(OUT)\source\fb\fbvidmux.obj
	  $(OUT)\source\fb\fbcortex.obj
	  $(OUT)\source\fb\fbdir.obj
	  $(OUT)\source\fb\fbgen.obj
	  $(OUT)\source\fb\fbmeteor.obj
	  $(OUT)\source\fb\fbbt.obj
	  $(OUT)\source\fb\fbvbmci.obj
	  $(OUT)\source\fb\fbvfw.obj
	  $(OUT)\source\fb\fbsnap.obj
	  $(OUT)\source\fb\fbutil.obj
	  $(OUT)\source\fb\libmain.obj
<<
	
$(OUT)\dynamic\dtfb.dll \
$(OUT)\libdll\dtfb.lib: \
	  $(OUT)\source\fb\fb.obj \
	  $(OUT)\source\fb\fbvidmux.obj \
	  $(OUT)\source\fb\fbcortex.obj \
	  $(OUT)\source\fb\fbdir.obj \
	  $(OUT)\source\fb\fbgen.obj \
	  $(OUT)\source\fb\fbmeteor.obj \
	  $(OUT)\source\fb\fbbt.obj \
	  $(OUT)\source\fb\fbvbmci.obj \
	  $(OUT)\source\fb\fbvfw.obj \
	  $(OUT)\source\fb\fbsnap.obj \
	  $(OUT)\source\fb\fbutil.obj \
	  $(OUT)\source\fb\libmain.obj \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtfb.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtfb.lib
	  $(OUT)\source\fb\fb.obj
	  $(OUT)\source\fb\fbvidmux.obj
	  $(OUT)\source\fb\fbcortex.obj
	  $(OUT)\source\fb\fbdir.obj
	  $(OUT)\source\fb\fbgen.obj
	  $(OUT)\source\fb\fbmeteor.obj
	  $(OUT)\source\fb\fbbt.obj
	  $(OUT)\source\fb\fbvbmci.obj
	  $(OUT)\source\fb\fbvfw.obj
	  $(OUT)\source\fb\fbsnap.obj
	  $(OUT)\source\fb\fbutil.obj
	  $(OUT)\source\fb\libmain.obj
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  $(MK_WINNOV_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\fb\fbcortex.obj: $(SRC)\source\fb\fbcortex.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\phys.h \
	  $(SRC)\include\dtack\phys.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbcortex.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbcortex.c
$(OUT)\source\fb\fbmeteor.obj: $(SRC)\source\fb\fbmeteor.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbmeteor.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_METEOR_DEFINE \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbmeteor.c
$(OUT)\source\fb\fbbt.obj: $(SRC)\source\fb\fbbt.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\portio.h \
	  $(SRC)\include\dtack\portio.p \
	  $(SRC)\include\dtack\vidmux.h \
	  $(SRC)\include\dtack\vidmux.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbbt.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_BTTV_DEFINE \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbbt.c
$(OUT)\source\fb\fbvbmci.obj: $(SRC)\source\fb\fbvbmci.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbvbmci.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_VBMCI=DT_1 \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbvbmci.c
$(OUT)\source\fb\fbutil.obj: $(SRC)\source\fb\fbutil.c \
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
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbutil.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbutil.c
$(OUT)\source\fb\fbvfw.obj: $(SRC)\source\fb\fbvfw.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\portio.h \
	  $(SRC)\include\dtack\portio.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbvfw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_VFW=DT_1 -DDTPRJ_WINNOV=DT_$(MK_WINNOV_DEFINE) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  -I$(MK_WINNOV_INCLUDE) \
	  $(SRC)\source\fb\fbvfw.c
$(OUT)\source\fb\fbsnap.obj: $(SRC)\source\fb\fbsnap.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\portio.h \
	  $(SRC)\include\dtack\portio.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbsnap.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_SNAP=DT_$(MK_SNAP_DEFINE) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbsnap.c
$(OUT)\source\fb\fb.obj: $(SRC)\source\fb\fb.c \
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
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fb.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fb.c
$(OUT)\source\fb\fbvidmux.obj: $(SRC)\source\fb\fbvidmux.c \
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
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\portio.h \
	  $(SRC)\include\dtack\portio.p \
	  $(SRC)\include\dtack\vidmux.h \
	  $(SRC)\include\dtack\vidmux.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbvidmux.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbvidmux.c
$(OUT)\source\fb\fbdir.obj: $(SRC)\source\fb\fbdir.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\phys.h \
	  $(SRC)\include\dtack\phys.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbdir.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbdir.c
$(OUT)\source\fb\fbgen.obj: $(SRC)\source\fb\fbgen.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\fb\fbgen.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\fbgen.c
$(OUT)\source\fb\libmain.obj: $(SRC)\source\fb\libmain.c \
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
	   /Fo$(OUT)\source\fb\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\fb \
	  -I$(SRC)\include \
	  $(SRC)\source\fb\libmain.c


