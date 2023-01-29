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
	  $(OUT)\static\dttinter.exe

shexes: \
	  $(OUT)\dynamic\dttinter.exe

scripts: 

libs: \
	  $(OUT)\lib\dttinter.lib

shlibs: \
	  $(OUT)\dynamic\dttinter.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\dttinter.exe
	-del $(OUT)\static\dttinter.exe
	-del $(OUT)\lib\dttinter.lib
	-del $(OUT)\dynamic\dttinter.dll
	-del $(OUT)\test\inter\*.obj
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



$(OUT)\static\dttinter.exe: \
	  $(OUT)\test\inter\inter.obj \
	  $(OUT)\lib\dttinter.lib \
	  $(OUT)\lib\dttzz.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttinter.exe @<<
	  /MAP:$(OUT)\static\dttinter.map
	  /PDB:$(OUT)\static\dttinter.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\inter\inter.obj
	$(OUT)\lib\dttinter.lib
	$(OUT)\lib\dttzz.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttinter.res
<<
$(OUT)\dynamic\dttinter.exe: \
	  $(OUT)\test\inter\inter.obj \
	  $(OUT)\libdll\dttinter.lib \
	  $(OUT)\libdll\dttzz.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttinter.exe @<<
	  /MAP:$(OUT)\dynamic\dttinter.map 
	  /PDB:$(OUT)\dynamic\dttinter.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\inter\inter.obj
	  $(OUT)\libdll\dttinter.lib
	  $(OUT)\libdll\dttzz.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttinter.lib
	del $(OUT)\dynamic\dttinter.exp



#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dttinter.lib: \
	  $(OUT)\test\inter\dttmap.obj \
	  $(OUT)\test\inter\dttkey.obj \
	  $(OUT)\test\inter\dttdia.obj \
	  $(OUT)\test\inter\dttalpha.obj \
	  $(OUT)\test\inter\dttnote.obj
	lib /nologo /out:$(OUT)\lib\dttinter.lib @<<
	  $(OUT)\test\inter\dttmap.obj
	  $(OUT)\test\inter\dttkey.obj
	  $(OUT)\test\inter\dttdia.obj
	  $(OUT)\test\inter\dttalpha.obj
	  $(OUT)\test\inter\dttnote.obj
<<
	
$(OUT)\dynamic\dttinter.dll \
$(OUT)\libdll\dttinter.lib: \
	  $(OUT)\test\inter\dttmap.obj \
	  $(OUT)\test\inter\dttkey.obj \
	  $(OUT)\test\inter\dttdia.obj \
	  $(OUT)\test\inter\dttalpha.obj \
	  $(OUT)\test\inter\dttnote.obj
	link /nologo /out:$(OUT)\dynamic\dttinter.dll @<<!
	  /dll /implib:$(OUT)\libdll\dttinter.lib
	  $(OUT)\test\inter\dttmap.obj
	  $(OUT)\test\inter\dttkey.obj
	  $(OUT)\test\inter\dttdia.obj
	  $(OUT)\test\inter\dttalpha.obj
	  $(OUT)\test\inter\dttnote.obj

	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\test\inter\dttmap.obj: $(SRC)\test\inter\dttmap.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\inter\dttmap.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\inter \
	  -I$(SRC)\include \
	  $(SRC)\test\inter\dttmap.c
$(OUT)\test\inter\dttkey.obj: $(SRC)\test\inter\dttkey.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\inter\dttkey.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\inter \
	  -I$(SRC)\include \
	  $(SRC)\test\inter\dttkey.c
$(OUT)\test\inter\dttdia.obj: $(SRC)\test\inter\dttdia.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\inter\dttdia.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\inter \
	  -I$(SRC)\include \
	  $(SRC)\test\inter\dttdia.c
$(OUT)\test\inter\dttalpha.obj: $(SRC)\test\inter\dttalpha.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\dialog.h \
	  $(SRC)\include\dtack\dialog.p \
	  $(SRC)\include\dtack\alpha.h \
	  $(SRC)\include\dtack\alpha.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\inter\dttalpha.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\inter \
	  -I$(SRC)\include \
	  $(SRC)\test\inter\dttalpha.c
$(OUT)\test\inter\dttnote.obj: $(SRC)\test\inter\dttnote.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\note.h \
	  $(SRC)\include\dtack\note.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\inter\dttnote.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\inter \
	  -I$(SRC)\include \
	  $(SRC)\test\inter\dttnote.c
$(OUT)\test\inter\inter.obj: $(SRC)\test\inter\inter.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\inter\inter.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\inter \
	  -I$(SRC)\include \
	  $(SRC)\test\inter\inter.c


