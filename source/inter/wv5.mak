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
	  $(OUT)\lib\dtinter.lib

shlibs: \
	  $(OUT)\dynamic\dtinter.dll

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\lib\dtinter.lib
	-del $(OUT)\dynamic\dtinter.dll
	-del $(OUT)\source\inter\*.obj
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



$(OUT)\lib\dtinter.lib: \
	  $(OUT)\source\inter\alpha.obj \
	  $(OUT)\source\inter\dialog.obj \
	  $(OUT)\source\inter\diatext.obj \
	  $(OUT)\source\inter\diavba.obj \
	  $(OUT)\source\inter\iact.obj \
	  $(OUT)\source\inter\interact.obj \
	  $(OUT)\source\inter\interdia.obj \
	  $(OUT)\source\inter\interhlp.obj \
	  $(OUT)\source\inter\note.obj \
	  $(OUT)\source\inter\text.obj \
	  $(OUT)\source\inter\textcur1.obj \
	  $(OUT)\source\inter\texttcap.obj \
	  $(OUT)\source\inter\textdos1.obj \
	  $(OUT)\source\inter\textqnx1.obj \
	  $(OUT)\source\inter\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtinter.lib @<<
	  $(OUT)\source\inter\alpha.obj
	  $(OUT)\source\inter\dialog.obj
	  $(OUT)\source\inter\diatext.obj
	  $(OUT)\source\inter\diavba.obj
	  $(OUT)\source\inter\iact.obj
	  $(OUT)\source\inter\interact.obj
	  $(OUT)\source\inter\interdia.obj
	  $(OUT)\source\inter\interhlp.obj
	  $(OUT)\source\inter\note.obj
	  $(OUT)\source\inter\text.obj
	  $(OUT)\source\inter\textcur1.obj
	  $(OUT)\source\inter\texttcap.obj
	  $(OUT)\source\inter\textdos1.obj
	  $(OUT)\source\inter\textqnx1.obj
	  $(OUT)\source\inter\libmain.obj
<<
	
$(OUT)\dynamic\dtinter.dll \
$(OUT)\libdll\dtinter.lib: \
	  $(OUT)\source\inter\alpha.obj \
	  $(OUT)\source\inter\dialog.obj \
	  $(OUT)\source\inter\diatext.obj \
	  $(OUT)\source\inter\diavba.obj \
	  $(OUT)\source\inter\iact.obj \
	  $(OUT)\source\inter\interact.obj \
	  $(OUT)\source\inter\interdia.obj \
	  $(OUT)\source\inter\interhlp.obj \
	  $(OUT)\source\inter\note.obj \
	  $(OUT)\source\inter\text.obj \
	  $(OUT)\source\inter\textcur1.obj \
	  $(OUT)\source\inter\texttcap.obj \
	  $(OUT)\source\inter\textdos1.obj \
	  $(OUT)\source\inter\textqnx1.obj \
	  $(OUT)\source\inter\libmain.obj \
	  $(OUT)\libdll\dtkeybd.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtinter.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtinter.lib
	  $(OUT)\source\inter\alpha.obj
	  $(OUT)\source\inter\dialog.obj
	  $(OUT)\source\inter\diatext.obj
	  $(OUT)\source\inter\diavba.obj
	  $(OUT)\source\inter\iact.obj
	  $(OUT)\source\inter\interact.obj
	  $(OUT)\source\inter\interdia.obj
	  $(OUT)\source\inter\interhlp.obj
	  $(OUT)\source\inter\note.obj
	  $(OUT)\source\inter\text.obj
	  $(OUT)\source\inter\textcur1.obj
	  $(OUT)\source\inter\texttcap.obj
	  $(OUT)\source\inter\textdos1.obj
	  $(OUT)\source\inter\textqnx1.obj
	  $(OUT)\source\inter\libmain.obj
	  $(OUT)\libdll\dtkeybd.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\inter\textcur1.obj: $(SRC)\source\inter\textcur1.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\text.h \
	  $(SRC)\include\dtack\text.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\textcur1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\textcur1.c
$(OUT)\source\inter\textdos1.obj: $(SRC)\source\inter\textdos1.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\text.h \
	  $(SRC)\include\dtack\text.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\textdos1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\textdos1.c
$(OUT)\source\inter\textqnx1.obj: $(SRC)\source\inter\textqnx1.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\text.h \
	  $(SRC)\include\dtack\text.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\qnx4.h \
	  $(SRC)\include\dtack\qnx4.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\textqnx1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\textqnx1.c
$(OUT)\source\inter\texttcap.obj: $(SRC)\source\inter\texttcap.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\text.h \
	  $(SRC)\include\dtack\text.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\texttcap.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_TERMCAP_DEFINE \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  -IPRJ_TERMCAP_INCLUDE \
	  $(SRC)\source\inter\texttcap.c
$(OUT)\source\inter\alpha.obj: $(SRC)\source\inter\alpha.c \
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
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\alpha.h \
	  $(SRC)\include\dtack\alpha.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\alpha.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\alpha.c
$(OUT)\source\inter\dialog.obj: $(SRC)\source\inter\dialog.c \
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
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\dialog.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\dialog.c
$(OUT)\source\inter\diatext.obj: $(SRC)\source\inter\diatext.c \
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
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\text.h \
	  $(SRC)\include\dtack\text.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\diatext.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\diatext.c
$(OUT)\source\inter\diavba.obj: $(SRC)\source\inter\diavba.c \
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
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\diavba.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\diavba.c
$(OUT)\source\inter\iact.obj: $(SRC)\source\inter\iact.c \
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
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\alpha.h \
	  $(SRC)\include\dtack\alpha.p \
	  $(SRC)\include\dtack\iact.h \
	  $(SRC)\include\dtack\iact.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\iact.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\iact.c
$(OUT)\source\inter\interact.obj: $(SRC)\source\inter\interact.c \
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
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\alpha.h \
	  $(SRC)\include\dtack\alpha.p \
	  $(SRC)\include\dtack\iact.h \
	  $(SRC)\include\dtack\iact.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\interact.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\interact.c
$(OUT)\source\inter\interdia.obj: $(SRC)\source\inter\interdia.c \
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
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\alpha.h \
	  $(SRC)\include\dtack\alpha.p \
	  $(SRC)\include\dtack\iact.h \
	  $(SRC)\include\dtack\iact.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\interdia.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\interdia.c
$(OUT)\source\inter\interhlp.obj: $(SRC)\source\inter\interhlp.c \
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
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\alpha.h \
	  $(SRC)\include\dtack\alpha.p \
	  $(SRC)\include\dtack\iact.h \
	  $(SRC)\include\dtack\iact.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\interhlp.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\interhlp.c
$(OUT)\source\inter\note.obj: $(SRC)\source\inter\note.c \
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
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\note.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\note.c
$(OUT)\source\inter\text.obj: $(SRC)\source\inter\text.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\text.h \
	  $(SRC)\include\dtack\text.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\inter\text.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\text.c
$(OUT)\source\inter\libmain.obj: $(SRC)\source\inter\libmain.c \
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
	   /Fo$(OUT)\source\inter\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\inter \
	  -I$(SRC)\include \
	  $(SRC)\source\inter\libmain.c


