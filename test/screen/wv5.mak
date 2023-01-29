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
	  $(OUT)\static\dttscr.exe

shexes: \
	  $(OUT)\dynamic\dttscr.exe

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
	-del $(OUT)\dynamic\dttscr.exe
	-del $(OUT)\static\dttscr.exe
	-del $(OUT)\test\screen\*.obj
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



$(OUT)\static\dttscr.exe: \
	  $(OUT)\test\screen\dttscr.obj \
	  $(OUT)\test\screen\dttscrx.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchgs.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtscr.lib \
	  $(OUT)\lib\dtcand.lib \
	  $(OUT)\lib\dthws.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttscr.exe @<<
	  /MAP:$(OUT)\static\dttscr.map
	  /PDB:$(OUT)\static\dttscr.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\screen\dttscr.obj
	$(OUT)\test\screen\dttscrx.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchgs.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtscr.lib
	$(OUT)\lib\dtcand.lib
	$(OUT)\lib\dthws.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_FGW_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttscr.res
<<
$(OUT)\dynamic\dttscr.exe: \
	  $(OUT)\test\screen\dttscr.obj \
	  $(OUT)\test\screen\dttscrx.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchgs.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtscr.lib \
	  $(OUT)\libdll\dtcand.lib \
	  $(OUT)\libdll\dthws.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttscr.exe @<<
	  /MAP:$(OUT)\dynamic\dttscr.map 
	  /PDB:$(OUT)\dynamic\dttscr.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\screen\dttscr.obj
	  $(OUT)\test\screen\dttscrx.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchgs.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtscr.lib
	  $(OUT)\libdll\dtcand.lib
	  $(OUT)\libdll\dthws.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_FGW_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttscr.lib
	del $(OUT)\dynamic\dttscr.exp
















#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\test\screen\dttscr.obj: $(SRC)\test\screen\dttscr.c \
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
	   /Fo$(OUT)\test\screen\dttscr.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\screen \
	  -I$(SRC)\include \
	  $(SRC)\test\screen\dttscr.c
$(OUT)\test\screen\dttscrx.obj: $(SRC)\test\screen\dttscrx.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\win_r.h \
	  $(SRC)\include\dtack\wins.h \
	  $(SRC)\include\dtack\wins.p \
	  $(SRC)\include\dtack\dsk.h \
	  $(SRC)\include\dtack\dsk.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\test\screen\dttscr.use
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\screen\dttscrx.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\screen \
	  -I$(SRC)\include \
	  $(SRC)\test\screen\dttscrx.c


