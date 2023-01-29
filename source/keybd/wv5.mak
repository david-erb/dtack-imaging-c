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
	  $(OUT)\static\dtkeybd1.exe

shexes: \
	  $(OUT)\dynamic\dtkeybd1.exe

scripts: 

libs: \
	  $(OUT)\lib\dtkeybd.lib

shlibs: \
	  $(OUT)\dynamic\dtkeybd.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\dtkeybd1.exe
	-del $(OUT)\static\dtkeybd1.exe
	-del $(OUT)\lib\dtkeybd.lib
	-del $(OUT)\dynamic\dtkeybd.dll
	-del $(OUT)\source\keybd\*.obj
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



$(OUT)\static\dtkeybd1.exe: \
	  $(OUT)\source\keybd\dtkeybd1.obj \
	  $(OUT)\lib\dtkeybd.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtkeybd1.exe @<<
	  /MAP:$(OUT)\static\dtkeybd1.map
	  /PDB:$(OUT)\static\dtkeybd1.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\keybd\dtkeybd1.obj
	$(OUT)\lib\dtkeybd.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtkeybd1.res
<<
$(OUT)\dynamic\dtkeybd1.exe: \
	  $(OUT)\source\keybd\dtkeybd1.obj \
	  $(OUT)\libdll\dtkeybd.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtkeybd1.exe @<<
	  /MAP:$(OUT)\dynamic\dtkeybd1.map 
	  /PDB:$(OUT)\dynamic\dtkeybd1.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\keybd\dtkeybd1.obj
	  $(OUT)\libdll\dtkeybd.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtkeybd1.lib
	del $(OUT)\dynamic\dtkeybd1.exp





#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dtkeybd.lib: \
	  $(OUT)\source\keybd\dikeybd.obj \
	  $(OUT)\source\keybd\keybd.obj \
	  $(OUT)\source\keybd\keyunix.obj \
	  $(OUT)\source\keybd\keyqnx.obj \
	  $(OUT)\source\keybd\keydos.obj \
	  $(OUT)\source\keybd\keymap.obj \
	  $(OUT)\source\keybd\keymapt.obj \
	  $(OUT)\source\keybd\keymapc.obj \
	  $(OUT)\source\keybd\keymapq.obj \
	  $(OUT)\source\keybd\trmios.obj \
	  $(OUT)\source\keybd\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtkeybd.lib \
	  $(OUT)\source\keybd\dikeybd.obj \
	  $(OUT)\source\keybd\keybd.obj \
	  $(OUT)\source\keybd\keyunix.obj \
	  $(OUT)\source\keybd\keyqnx.obj \
	  $(OUT)\source\keybd\keydos.obj \
	  $(OUT)\source\keybd\keymap.obj \
	  $(OUT)\source\keybd\keymapt.obj \
	  $(OUT)\source\keybd\keymapc.obj \
	  $(OUT)\source\keybd\keymapq.obj \
	  $(OUT)\source\keybd\trmios.obj \
	  $(OUT)\source\keybd\libmain.obj
	
$(OUT)\dynamic\dtkeybd.dll \
$(OUT)\libdll\dtkeybd.lib: \
	  $(OUT)\source\keybd\dikeybd.obj \
	  $(OUT)\source\keybd\keybd.obj \
	  $(OUT)\source\keybd\keyunix.obj \
	  $(OUT)\source\keybd\keyqnx.obj \
	  $(OUT)\source\keybd\keydos.obj \
	  $(OUT)\source\keybd\keymap.obj \
	  $(OUT)\source\keybd\keymapt.obj \
	  $(OUT)\source\keybd\keymapc.obj \
	  $(OUT)\source\keybd\keymapq.obj \
	  $(OUT)\source\keybd\trmios.obj \
	  $(OUT)\source\keybd\libmain.obj \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\dtkeybd.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtkeybd.lib
	  $(OUT)\source\keybd\keybd.obj
	  $(OUT)\source\keybd\keyunix.obj
	  $(OUT)\source\keybd\keyqnx.obj
	  $(OUT)\source\keybd\keydos.obj
	  $(OUT)\source\keybd\keymap.obj
	  $(OUT)\source\keybd\keymapt.obj
	  $(OUT)\source\keybd\keymapc.obj
	  $(OUT)\source\keybd\keymapq.obj
	  $(OUT)\source\keybd\trmios.obj
	  $(OUT)\source\keybd\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\keybd\dtkeybd1.obj: $(SRC)\source\keybd\dtkeybd1.c \
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
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\dtkeybd1.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\dtkeybd1.c
$(OUT)\source\keybd\keymapq.obj: $(SRC)\source\keybd\keymapq.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keymapq.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  -IPRJ_QNXTERM_INCLUDE \
	  $(SRC)\source\keybd\keymapq.c
$(OUT)\source\keybd\keymapt.obj: $(SRC)\source\keybd\keymapt.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keymapt.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_TERMCAP_DEFINE) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  -I$(MK_TERMCAP_INCLUDE) \
	  $(SRC)\source\keybd\keymapt.c
$(OUT)\source\keybd\keyunix.obj: $(SRC)\source\keybd\keyunix.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keyunix.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_TERMIOS_DEFINE) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  -I$(MK_TERMIOS_INCLUDE) \
	  $(SRC)\source\keybd\keyunix.c
$(OUT)\source\keybd\keyqnx.obj: $(SRC)\source\keybd\keyqnx.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keyqnx.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  -IPRJ_QNXTERM_INCLUDE \
	  $(SRC)\source\keybd\keyqnx.c
$(OUT)\source\keybd\trmios.obj: $(SRC)\source\keybd\trmios.c \
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
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\trmios.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_TERMIOS_DEFINE) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  -I$(MK_TERMIOS_INCLUDE) \
	  $(SRC)\source\keybd\trmios.c
$(OUT)\source\keybd\dikeybd.obj: $(SRC)\source\keybd\dikeybd.c \
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
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /TP /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\dikeybd.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -Ic:\progra~1\directx\8.1b\include \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\dikeybd.c
$(OUT)\source\keybd\keybd.obj: $(SRC)\source\keybd\keybd.c \
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
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keybd.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\keybd.c
$(OUT)\source\keybd\keydos.obj: $(SRC)\source\keybd\keydos.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keydos.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\keydos.c
$(OUT)\source\keybd\keymap.obj: $(SRC)\source\keybd\keymap.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keymap.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\keymap.c
$(OUT)\source\keybd\keymapc.obj: $(SRC)\source\keybd\keymapc.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\keybd\keymapc.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\keymapc.c
$(OUT)\source\keybd\libmain.obj: $(SRC)\source\keybd\libmain.c \
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
	   /Fo$(OUT)\source\keybd\libmain.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\keybd \
	  -I$(SRC)\include \
	  $(SRC)\source\keybd\libmain.c


