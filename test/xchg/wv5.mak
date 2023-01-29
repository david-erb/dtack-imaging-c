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
statics: exes 

shared: shareds
shareds: shexes 

allsubs:

exes: \
	  $(OUT)\static\dttxchg.exe

shexes: \
	  $(OUT)\dynamic\dttxchg.exe

scripts: 

libs: 

shlibs: 

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\dttxchg.exe
	-del $(OUT)\static\dttxchg.exe
	-del $(OUT)\test\xchg\*.obj
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



$(OUT)\static\dttxchg.exe: \
	  $(OUT)\test\xchg\dttxchg.obj \
	  $(OUT)\test\xchg\dttxchg2.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttxchg.exe @<<
	  /MAP:$(OUT)\static\dttxchg.map
	  /PDB:$(OUT)\static\dttxchg.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\xchg\dttxchg.obj
	$(OUT)\test\xchg\dttxchg2.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttxchg.res
<<
$(OUT)\dynamic\dttxchg.exe: \
	  $(OUT)\test\xchg\dttxchg.obj \
	  $(OUT)\test\xchg\dttxchg2.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttxchg.exe @<<
	  /MAP:$(OUT)\dynamic\dttxchg.map 
	  /PDB:$(OUT)\dynamic\dttxchg.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\xchg\dttxchg.obj
	  $(OUT)\test\xchg\dttxchg2.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttxchg.lib
	del $(OUT)\dynamic\dttxchg.exp



#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\test\xchg\dttxchg.obj: $(SRC)\test\xchg\dttxchg.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\dsk.h \
	  $(SRC)\include\dtack\dsk.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\xchg\dttxchg.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\xchg \
	  -I$(SRC)\include \
	  $(SRC)\test\xchg\dttxchg.c
$(OUT)\test\xchg\dttxchg2.obj: $(SRC)\test\xchg\dttxchg2.c \
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
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\xchg\dttxchg2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\xchg \
	  -I$(SRC)\include \
	  $(SRC)\test\xchg\dttxchg2.c


