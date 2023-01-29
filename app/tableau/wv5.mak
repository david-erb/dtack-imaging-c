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
	  $(OUT)\static\tableau.exe

shexes: \
	  $(OUT)\dynamic\tableau.exe

scripts: 

libs: \
	  $(OUT)\lib\tableaux.lib

shlibs: \
	  $(OUT)\dynamic\tableaux.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\tableau.exe
	-del $(OUT)\static\tableau.exe
	-del $(OUT)\lib\tableaux.lib
	-del $(OUT)\dynamic\tableaux.dll
	-del $(OUT)\app\tableau\*.obj
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



$(OUT)\static\tableau.exe: \
	  $(OUT)\app\tableau\main.obj \
	  $(OUT)\app\tableau\demo0.obj \
	  $(OUT)\lib\tableaux.lib \
	  $(OUT)\lib\dschedx.lib \
	  $(OUT)\lib\reverbx.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtxchgs.lib \
	  $(OUT)\lib\dtfb.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dtdg.lib \
	  $(OUT)\lib\dtscr.lib \
	  $(OUT)\lib\dtcand.lib \
	  $(OUT)\lib\dthws.lib \
	  $(OUT)\lib\dtfastgd.lib \
	  $(OUT)\lib\dtsvgacd.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\tableau.exe @<<
	  /MAP:$(OUT)\static\tableau.map
	  /PDB:$(OUT)\static\tableau.pdb
 /DEBUG /STACK:256000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\tableau\main.obj
	$(OUT)\app\tableau\demo0.obj
	$(OUT)\lib\tableaux.lib
	$(OUT)\lib\dschedx.lib
	$(OUT)\lib\reverbx.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtxchgs.lib
	$(OUT)\lib\dtfb.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dtdg.lib
	$(OUT)\lib\dtscr.lib
	$(OUT)\lib\dtcand.lib
	$(OUT)\lib\dthws.lib
	$(OUT)\lib\dtfastgd.lib
	$(OUT)\lib\dtsvgacd.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_WINNOV_LOAD)
  $(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_FGW_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\tableau.res
<<
$(OUT)\dynamic\tableau.exe: \
	  $(OUT)\app\tableau\main.obj \
	  $(OUT)\app\tableau\demo0.obj \
	  $(OUT)\libdll\tableaux.lib \
	  $(OUT)\libdll\dschedx.lib \
	  $(OUT)\libdll\reverbx.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtxchgs.lib \
	  $(OUT)\libdll\dtfb.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dtdg.lib \
	  $(OUT)\libdll\dtscr.lib \
	  $(OUT)\libdll\dtcand.lib \
	  $(OUT)\libdll\dthws.lib \
	  $(OUT)\libdll\dtfastgd.lib \
	  $(OUT)\libdll\dtsvgacd.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\tableau.exe @<<
	  /MAP:$(OUT)\dynamic\tableau.map 
	  /PDB:$(OUT)\dynamic\tableau.pdb 

	  /DEBUG
	  /STACK:256000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\tableau\main.obj
	  $(OUT)\app\tableau\demo0.obj
	  $(OUT)\libdll\tableaux.lib
	  $(OUT)\libdll\dschedx.lib
	  $(OUT)\libdll\reverbx.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtxchgs.lib
	  $(OUT)\libdll\dtfb.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dtdg.lib
	  $(OUT)\libdll\dtscr.lib
	  $(OUT)\libdll\dtcand.lib
	  $(OUT)\libdll\dthws.lib
	  $(OUT)\libdll\dtfastgd.lib
	  $(OUT)\libdll\dtsvgacd.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_WINNOV_LOAD)
  $(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_FGW_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\tableau.lib
	del $(OUT)\dynamic\tableau.exp





#-------------------------------------------------------------------------
# libs



$(OUT)\lib\tableaux.lib: \
	  $(OUT)\app\tableau\tableau.obj \
	  $(OUT)\app\tableau\dialog.obj \
	  $(OUT)\app\tableau\demo0.obj \
	  $(OUT)\app\tableau\annotate.obj \
	  $(OUT)\app\tableau\cam.obj \
	  $(OUT)\app\tableau\cook.obj \
	  $(OUT)\app\tableau\data.obj \
	  $(OUT)\app\tableau\decode.obj \
	  $(OUT)\app\tableau\dir.obj \
	  $(OUT)\app\tableau\encode.obj \
	  $(OUT)\app\tableau\fast.obj \
	  $(OUT)\app\tableau\get.obj \
	  $(OUT)\app\tableau\get_fb.obj \
	  $(OUT)\app\tableau\get_roll.obj \
	  $(OUT)\app\tableau\get_ser.obj \
	  $(OUT)\app\tableau\get_file.obj \
	  $(OUT)\app\tableau\http.obj \
	  $(OUT)\app\tableau\ipc.obj \
	  $(OUT)\app\tableau\log.obj \
	  $(OUT)\app\tableau\poll.obj \
	  $(OUT)\app\tableau\put.obj \
	  $(OUT)\app\tableau\put_roll.obj \
	  $(OUT)\app\tableau\put_ftp.obj \
	  $(OUT)\app\tableau\put_ipc.obj \
	  $(OUT)\app\tableau\put_file.obj \
	  $(OUT)\app\tableau\put_push.obj \
	  $(OUT)\app\tableau\put_scr.obj \
	  $(OUT)\app\tableau\rev.obj \
	  $(OUT)\app\tableau\roll.obj \
	  $(OUT)\app\tableau\shovel.obj \
	  $(OUT)\app\tableau\test.obj \
	  $(OUT)\app\tableau\libmain.obj
	lib /nologo /out:$(OUT)\lib\tableaux.lib @<<
	  $(OUT)\app\tableau\tableau.obj
	  $(OUT)\app\tableau\dialog.obj
	  $(OUT)\app\tableau\demo0.obj
	  $(OUT)\app\tableau\annotate.obj
	  $(OUT)\app\tableau\cam.obj
	  $(OUT)\app\tableau\cook.obj
	  $(OUT)\app\tableau\data.obj
	  $(OUT)\app\tableau\decode.obj
	  $(OUT)\app\tableau\dir.obj
	  $(OUT)\app\tableau\encode.obj
	  $(OUT)\app\tableau\fast.obj
	  $(OUT)\app\tableau\get.obj
	  $(OUT)\app\tableau\get_fb.obj
	  $(OUT)\app\tableau\get_roll.obj
	  $(OUT)\app\tableau\get_ser.obj
	  $(OUT)\app\tableau\get_file.obj
	  $(OUT)\app\tableau\http.obj
	  $(OUT)\app\tableau\ipc.obj
	  $(OUT)\app\tableau\log.obj
	  $(OUT)\app\tableau\poll.obj
	  $(OUT)\app\tableau\put.obj
	  $(OUT)\app\tableau\put_roll.obj
	  $(OUT)\app\tableau\put_ftp.obj
	  $(OUT)\app\tableau\put_ipc.obj
	  $(OUT)\app\tableau\put_file.obj
	  $(OUT)\app\tableau\put_push.obj
	  $(OUT)\app\tableau\put_scr.obj
	  $(OUT)\app\tableau\rev.obj
	  $(OUT)\app\tableau\roll.obj
	  $(OUT)\app\tableau\shovel.obj
	  $(OUT)\app\tableau\test.obj
	  $(OUT)\app\tableau\libmain.obj
<<
	
$(OUT)\dynamic\tableaux.dll \
$(OUT)\libdll\tableaux.lib: \
	  $(OUT)\app\tableau\tableau.obj \
	  $(OUT)\app\tableau\dialog.obj \
	  $(OUT)\app\tableau\demo0.obj \
	  $(OUT)\app\tableau\annotate.obj \
	  $(OUT)\app\tableau\cam.obj \
	  $(OUT)\app\tableau\cook.obj \
	  $(OUT)\app\tableau\data.obj \
	  $(OUT)\app\tableau\decode.obj \
	  $(OUT)\app\tableau\dir.obj \
	  $(OUT)\app\tableau\encode.obj \
	  $(OUT)\app\tableau\fast.obj \
	  $(OUT)\app\tableau\get.obj \
	  $(OUT)\app\tableau\get_fb.obj \
	  $(OUT)\app\tableau\get_roll.obj \
	  $(OUT)\app\tableau\get_ser.obj \
	  $(OUT)\app\tableau\get_file.obj \
	  $(OUT)\app\tableau\http.obj \
	  $(OUT)\app\tableau\ipc.obj \
	  $(OUT)\app\tableau\log.obj \
	  $(OUT)\app\tableau\poll.obj \
	  $(OUT)\app\tableau\put.obj \
	  $(OUT)\app\tableau\put_roll.obj \
	  $(OUT)\app\tableau\put_ftp.obj \
	  $(OUT)\app\tableau\put_ipc.obj \
	  $(OUT)\app\tableau\put_file.obj \
	  $(OUT)\app\tableau\put_push.obj \
	  $(OUT)\app\tableau\put_scr.obj \
	  $(OUT)\app\tableau\rev.obj \
	  $(OUT)\app\tableau\roll.obj \
	  $(OUT)\app\tableau\shovel.obj \
	  $(OUT)\app\tableau\test.obj \
	  $(OUT)\app\tableau\libmain.obj \
	  $(OUT)\libdll\dschedx.lib \
	  $(OUT)\libdll\reverbx.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtxchgs.lib \
	  $(OUT)\libdll\dtfb.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dtscr.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtdg.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\tableaux.dll @<<!
	  /dll /implib:$(OUT)\libdll\tableaux.lib
	  $(OUT)\app\tableau\tableau.obj
	  $(OUT)\app\tableau\dialog.obj
	  $(OUT)\app\tableau\demo0.obj
	  $(OUT)\app\tableau\annotate.obj
	  $(OUT)\app\tableau\cam.obj
	  $(OUT)\app\tableau\cook.obj
	  $(OUT)\app\tableau\data.obj
	  $(OUT)\app\tableau\decode.obj
	  $(OUT)\app\tableau\dir.obj
	  $(OUT)\app\tableau\encode.obj
	  $(OUT)\app\tableau\fast.obj
	  $(OUT)\app\tableau\get.obj
	  $(OUT)\app\tableau\get_fb.obj
	  $(OUT)\app\tableau\get_roll.obj
	  $(OUT)\app\tableau\get_ser.obj
	  $(OUT)\app\tableau\get_file.obj
	  $(OUT)\app\tableau\http.obj
	  $(OUT)\app\tableau\ipc.obj
	  $(OUT)\app\tableau\log.obj
	  $(OUT)\app\tableau\poll.obj
	  $(OUT)\app\tableau\put.obj
	  $(OUT)\app\tableau\put_roll.obj
	  $(OUT)\app\tableau\put_ftp.obj
	  $(OUT)\app\tableau\put_ipc.obj
	  $(OUT)\app\tableau\put_file.obj
	  $(OUT)\app\tableau\put_push.obj
	  $(OUT)\app\tableau\put_scr.obj
	  $(OUT)\app\tableau\rev.obj
	  $(OUT)\app\tableau\roll.obj
	  $(OUT)\app\tableau\shovel.obj
	  $(OUT)\app\tableau\test.obj
	  $(OUT)\app\tableau\libmain.obj
	  $(OUT)\libdll\dschedx.lib
	  $(OUT)\libdll\reverbx.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtxchgs.lib
	  $(OUT)\libdll\dtfb.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dtscr.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtdg.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\app\tableau\main.obj: $(SRC)\app\tableau\main.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\main.h
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\main.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\main.c
$(OUT)\app\tableau\fast.obj: $(SRC)\app\tableau\fast.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\fast.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\fast.c
$(OUT)\app\tableau\cam.obj: $(SRC)\app\tableau\cam.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\cam.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  -I$(SRC)\app\tabchief \
	  $(SRC)\app\tableau\cam.c
$(OUT)\app\tableau\test.obj: $(SRC)\app\tableau\test.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\test.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  -I$(SRC)\app\tabchief \
	  $(SRC)\app\tableau\test.c
$(OUT)\app\tableau\tableau.obj: $(SRC)\app\tableau\tableau.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\licens.h \
	  $(SRC)\app\tableau\tableau.use
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\tableau.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\tableau.c
$(OUT)\app\tableau\dialog.obj: $(SRC)\app\tableau\dialog.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\dialog.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\dialog.c
$(OUT)\app\tableau\demo0.obj: $(SRC)\app\tableau\demo0.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\demo0.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\demo0.c
$(OUT)\app\tableau\annotate.obj: $(SRC)\app\tableau\annotate.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\annotate.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\annotate.c
$(OUT)\app\tableau\cook.obj: $(SRC)\app\tableau\cook.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\cook.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\cook.c
$(OUT)\app\tableau\data.obj: $(SRC)\app\tableau\data.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\data.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\data.c
$(OUT)\app\tableau\decode.obj: $(SRC)\app\tableau\decode.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\decode.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\decode.c
$(OUT)\app\tableau\dir.obj: $(SRC)\app\tableau\dir.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\dir.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\dir.c
$(OUT)\app\tableau\encode.obj: $(SRC)\app\tableau\encode.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\app\tableau\mangle.h
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\encode.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\encode.c
$(OUT)\app\tableau\get.obj: $(SRC)\app\tableau\get.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\get.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\get.c
$(OUT)\app\tableau\get_fb.obj: $(SRC)\app\tableau\get_fb.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\get_fb.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\get_fb.c
$(OUT)\app\tableau\get_roll.obj: $(SRC)\app\tableau\get_roll.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\get_roll.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\get_roll.c
$(OUT)\app\tableau\get_ser.obj: $(SRC)\app\tableau\get_ser.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\yield.h
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\get_ser.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\get_ser.c
$(OUT)\app\tableau\get_file.obj: $(SRC)\app\tableau\get_file.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\get_file.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\get_file.c
$(OUT)\app\tableau\http.obj: $(SRC)\app\tableau\http.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\http.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\http.c
$(OUT)\app\tableau\ipc.obj: $(SRC)\app\tableau\ipc.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\ipc.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\ipc.c
$(OUT)\app\tableau\log.obj: $(SRC)\app\tableau\log.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\log.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\log.c
$(OUT)\app\tableau\poll.obj: $(SRC)\app\tableau\poll.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\poll.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\poll.c
$(OUT)\app\tableau\put.obj: $(SRC)\app\tableau\put.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put.c
$(OUT)\app\tableau\put_roll.obj: $(SRC)\app\tableau\put_roll.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put_roll.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put_roll.c
$(OUT)\app\tableau\put_ftp.obj: $(SRC)\app\tableau\put_ftp.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put_ftp.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put_ftp.c
$(OUT)\app\tableau\put_ipc.obj: $(SRC)\app\tableau\put_ipc.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put_ipc.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put_ipc.c
$(OUT)\app\tableau\put_file.obj: $(SRC)\app\tableau\put_file.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put_file.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put_file.c
$(OUT)\app\tableau\put_push.obj: $(SRC)\app\tableau\put_push.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put_push.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put_push.c
$(OUT)\app\tableau\put_scr.obj: $(SRC)\app\tableau\put_scr.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\put_scr.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\put_scr.c
$(OUT)\app\tableau\rev.obj: $(SRC)\app\tableau\rev.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\rev.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\rev.c
$(OUT)\app\tableau\roll.obj: $(SRC)\app\tableau\roll.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\roll.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\roll.c
$(OUT)\app\tableau\shovel.obj: $(SRC)\app\tableau\shovel.c \
	  $(SRC)\app\tableau\tableau.h \
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
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\dg.h \
	  $(SRC)\include\dtack\dg.p \
	  $(SRC)\include\dtack\xchg.h \
	  $(SRC)\include\dtack\xchg.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\color.h \
	  $(SRC)\include\dtack\color.p \
	  $(SRC)\include\dtack\fb.h \
	  $(SRC)\include\dtack\fb.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tableau\tableau.p \
	  $(SRC)\include\dtack\yield.h
	cl /nologo /c /Gf /J /GB /MDd /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tableau\shovel.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\shovel.c
$(OUT)\app\tableau\libmain.obj: $(SRC)\app\tableau\libmain.c \
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
	   /Fo$(OUT)\app\tableau\libmain.obj /Z7 /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tableau \
	  -I$(SRC)\include \
	  $(SRC)\app\tableau\libmain.c


