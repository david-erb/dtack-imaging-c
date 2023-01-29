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
	  $(OUT)\lib\dtscr.lib \
	  $(OUT)\lib\dtcan.lib \
	  $(OUT)\lib\dtcand.lib \
	  $(OUT)\lib\dtfastg.lib \
	  $(OUT)\lib\dtfastgd.lib \
	  $(OUT)\lib\dtsvgac.lib \
	  $(OUT)\lib\dtsvgacd.lib \
	  $(OUT)\lib\dthws.lib \
	  $(OUT)\lib\dthwsd.lib

shlibs: \
	  $(OUT)\dynamic\dtscr.dll \
	  $(OUT)\dynamic\dtcan.dll \
	  $(OUT)\dynamic\dtcand.dll \
	  $(OUT)\dynamic\dtfastg.dll \
	  $(OUT)\dynamic\dtfastgd.dll \
	  $(OUT)\dynamic\dtsvgac.dll \
	  $(OUT)\dynamic\dtsvgacd.dll \
	  $(OUT)\dynamic\dthws.dll \
	  $(OUT)\dynamic\dthwsd.dll

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\lib\dtscr.lib
	-del $(OUT)\lib\dtcan.lib
	-del $(OUT)\lib\dtcand.lib
	-del $(OUT)\lib\dtfastg.lib
	-del $(OUT)\lib\dtfastgd.lib
	-del $(OUT)\lib\dtsvgac.lib
	-del $(OUT)\lib\dtsvgacd.lib
	-del $(OUT)\lib\dthws.lib
	-del $(OUT)\lib\dthwsd.lib
	-del $(OUT)\dynamic\dtscr.dll
	-del $(OUT)\dynamic\dtcan.dll
	-del $(OUT)\dynamic\dtcand.dll
	-del $(OUT)\dynamic\dtfastg.dll
	-del $(OUT)\dynamic\dtfastgd.dll
	-del $(OUT)\dynamic\dtsvgac.dll
	-del $(OUT)\dynamic\dtsvgacd.dll
	-del $(OUT)\dynamic\dthws.dll
	-del $(OUT)\dynamic\dthwsd.dll
	-del $(OUT)\source\screen\*.obj
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



$(OUT)\lib\dtscr.lib: \
	  $(OUT)\source\screen\scr_main.obj \
	  $(OUT)\source\screen\scr_cur.obj \
	  $(OUT)\source\screen\scr_act.obj \
	  $(OUT)\source\screen\scr_dbg.obj \
	  $(OUT)\source\screen\scrdum.obj \
	  $(OUT)\source\screen\vga.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtscr.lib @<<
	  $(OUT)\source\screen\scr_main.obj
	  $(OUT)\source\screen\scr_cur.obj
	  $(OUT)\source\screen\scr_act.obj
	  $(OUT)\source\screen\scr_dbg.obj
	  $(OUT)\source\screen\scrdum.obj
	  $(OUT)\source\screen\vga.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtscr.dll \
$(OUT)\libdll\dtscr.lib: \
	  $(OUT)\source\screen\scr_main.obj \
	  $(OUT)\source\screen\scr_cur.obj \
	  $(OUT)\source\screen\scr_act.obj \
	  $(OUT)\source\screen\scr_dbg.obj \
	  $(OUT)\source\screen\scrdum.obj \
	  $(OUT)\source\screen\vga.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtcan.lib \
	  $(OUT)\libdll\dthws.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtscr.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtscr.lib
	  $(OUT)\source\screen\scr_main.obj
	  $(OUT)\source\screen\scr_cur.obj
	  $(OUT)\source\screen\scr_act.obj
	  $(OUT)\source\screen\scr_dbg.obj
	  $(OUT)\source\screen\scrdum.obj
	  $(OUT)\source\screen\vga.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtcan.lib
	  $(OUT)\libdll\dthws.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtcan.lib: \
	  $(OUT)\source\screen\scrcan.obj \
	  $(OUT)\source\screen\canvas.obj \
	  $(OUT)\source\screen\canpoll.obj \
	  $(OUT)\source\screen\candum.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtcan.lib @<<
	  $(OUT)\source\screen\scrcan.obj
	  $(OUT)\source\screen\canvas.obj
	  $(OUT)\source\screen\canpoll.obj
	  $(OUT)\source\screen\candum.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtcan.dll \
$(OUT)\libdll\dtcan.lib: \
	  $(OUT)\source\screen\scrcan.obj \
	  $(OUT)\source\screen\canvas.obj \
	  $(OUT)\source\screen\canpoll.obj \
	  $(OUT)\source\screen\candum.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtkeybd.lib \
	  $(OUT)\libdll\dtfastg.lib \
	  $(OUT)\libdll\dtsvgac.lib \
	  $(OUT)\libdll\dthws.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtcan.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtcan.lib
	  $(OUT)\source\screen\scrcan.obj
	  $(OUT)\source\screen\canvas.obj
	  $(OUT)\source\screen\canpoll.obj
	  $(OUT)\source\screen\candum.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtkeybd.lib
	  $(OUT)\libdll\dtfastg.lib
	  $(OUT)\libdll\dtsvgac.lib
	  $(OUT)\libdll\dthws.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtcand.lib: \
	  $(OUT)\source\screen\scrcand.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtcand.lib @<<
	  $(OUT)\source\screen\scrcand.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtcand.dll \
$(OUT)\libdll\dtcand.lib: \
	  $(OUT)\source\screen\scrcand.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtcand.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtcand.lib
	  $(OUT)\source\screen\scrcand.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtfastg.lib: \
	  $(OUT)\source\screen\canfastg.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtfastg.lib @<<
	  $(OUT)\source\screen\canfastg.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtfastg.dll \
$(OUT)\libdll\dtfastg.lib: \
	  $(OUT)\source\screen\canfastg.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtfastg.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtfastg.lib
	  $(OUT)\source\screen\canfastg.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtfastgd.lib: \
	  $(OUT)\source\screen\canfastd.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtfastgd.lib @<<
	  $(OUT)\source\screen\canfastd.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtfastgd.dll \
$(OUT)\libdll\dtfastgd.lib: \
	  $(OUT)\source\screen\canfastd.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtfastgd.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtfastgd.lib
	  $(OUT)\source\screen\canfastd.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtsvgac.lib: \
	  $(OUT)\source\screen\cansvgac.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtsvgac.lib @<<
	  $(OUT)\source\screen\cansvgac.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtsvgac.dll \
$(OUT)\libdll\dtsvgac.lib: \
	  $(OUT)\source\screen\cansvgac.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtsvgac.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtsvgac.lib
	  $(OUT)\source\screen\cansvgac.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtsvgacd.lib: \
	  $(OUT)\source\screen\cansvgad.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtsvgacd.lib @<<
	  $(OUT)\source\screen\cansvgad.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dtsvgacd.dll \
$(OUT)\libdll\dtsvgacd.lib: \
	  $(OUT)\source\screen\cansvgad.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtsvgacd.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtsvgacd.lib
	  $(OUT)\source\screen\cansvgad.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dthws.lib: \
	  $(OUT)\source\screen\scrhws.obj \
	  $(OUT)\source\screen\canhws.obj \
	  $(OUT)\source\screen\hws.obj \
	  $(OUT)\source\screen\hwsx.obj \
	  $(OUT)\source\screen\hwsqw4.obj \
	  $(OUT)\source\screen\hwsfgw.obj \
	  $(OUT)\source\screen\hwsmsw.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dthws.lib @<<
	  $(OUT)\source\screen\scrhws.obj
	  $(OUT)\source\screen\canhws.obj
	  $(OUT)\source\screen\hws.obj
	  $(OUT)\source\screen\hwsx.obj
	  $(OUT)\source\screen\hwsqw4.obj
	  $(OUT)\source\screen\hwsfgw.obj
	  $(OUT)\source\screen\hwsmsw.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dthws.dll \
$(OUT)\libdll\dthws.lib: \
	  $(OUT)\source\screen\scrhws.obj \
	  $(OUT)\source\screen\canhws.obj \
	  $(OUT)\source\screen\hws.obj \
	  $(OUT)\source\screen\hwsx.obj \
	  $(OUT)\source\screen\hwsqw4.obj \
	  $(OUT)\source\screen\hwsfgw.obj \
	  $(OUT)\source\screen\hwsmsw.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dthws.dll @<<!
	  /dll /implib:$(OUT)\libdll\dthws.lib
	  $(OUT)\source\screen\scrhws.obj
	  $(OUT)\source\screen\canhws.obj
	  $(OUT)\source\screen\hws.obj
	  $(OUT)\source\screen\hwsx.obj
	  $(OUT)\source\screen\hwsqw4.obj
	  $(OUT)\source\screen\hwsfgw.obj
	  $(OUT)\source\screen\hwsmsw.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)    $(MK_FGW_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dthwsd.lib: \
	  $(OUT)\source\screen\scrhwsd.obj \
	  $(OUT)\source\screen\canhwsd.obj \
	  $(OUT)\source\screen\libmain.obj
	lib /nologo /out:$(OUT)\lib\dthwsd.lib @<<
	  $(OUT)\source\screen\scrhwsd.obj
	  $(OUT)\source\screen\canhwsd.obj
	  $(OUT)\source\screen\libmain.obj
<<
	
$(OUT)\dynamic\dthwsd.dll \
$(OUT)\libdll\dthwsd.lib: \
	  $(OUT)\source\screen\scrhwsd.obj \
	  $(OUT)\source\screen\canhwsd.obj \
	  $(OUT)\source\screen\libmain.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dthwsd.dll @<<!
	  /dll /implib:$(OUT)\libdll\dthwsd.lib
	  $(OUT)\source\screen\scrhwsd.obj
	  $(OUT)\source\screen\canhwsd.obj
	  $(OUT)\source\screen\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\screen\canfastg.obj: $(SRC)\source\screen\canfastg.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p \
	  $(SRC)\include\dtack\vga.h \
	  $(SRC)\include\dtack\vga.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\nofar.h \
	  $(SRC)\include\dtack\fastg.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\canfastg.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\canfastg.c
$(OUT)\source\screen\cansvgac.obj: $(SRC)\source\screen\cansvgac.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p \
	  $(SRC)\include\dtack\vga.h \
	  $(SRC)\include\dtack\vga.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\nofar.h \
	  $(SRC)\include\dtack\svgacc.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\cansvgac.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\cansvgac.c
$(OUT)\source\screen\hwsx.obj: $(SRC)\source\screen\hwsx.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\hwsx.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\hwsx.c
$(OUT)\source\screen\hwsqw4.obj: $(SRC)\source\screen\hwsqw4.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\hwsqw4.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\hwsqw4.c
$(OUT)\source\screen\hwsfgw.obj: $(SRC)\source\screen\hwsfgw.c \
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
	  $(SRC)\include\dtack\label.h \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\hwsfgw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_FGW=DT_$(MK_FGW_DEFINE) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  -I$(MK_FGW_INCLUDE) \
	  $(SRC)\source\screen\hwsfgw.c
$(OUT)\source\screen\scr_main.obj: $(SRC)\source\screen\scr_main.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\font.h \
	  $(SRC)\include\dtack\font.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scr_main.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scr_main.c
$(OUT)\source\screen\scr_cur.obj: $(SRC)\source\screen\scr_cur.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scr_cur.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scr_cur.c
$(OUT)\source\screen\scr_act.obj: $(SRC)\source\screen\scr_act.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scr_act.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scr_act.c
$(OUT)\source\screen\scr_dbg.obj: $(SRC)\source\screen\scr_dbg.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scr_dbg.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scr_dbg.c
$(OUT)\source\screen\scrdum.obj: $(SRC)\source\screen\scrdum.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scrdum.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scrdum.c
$(OUT)\source\screen\vga.obj: $(SRC)\source\screen\vga.c \
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
	  $(SRC)\include\dtack\vga.h \
	  $(SRC)\include\dtack\vga.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\vga.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\vga.c
$(OUT)\source\screen\libmain.obj: $(SRC)\source\screen\libmain.c \
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
	   /Fo$(OUT)\source\screen\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\libmain.c
$(OUT)\source\screen\scrcan.obj: $(SRC)\source\screen\scrcan.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scrcan.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scrcan.c
$(OUT)\source\screen\canvas.obj: $(SRC)\source\screen\canvas.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\canvas.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\canvas.c
$(OUT)\source\screen\canpoll.obj: $(SRC)\source\screen\canpoll.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\canpoll.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\canpoll.c
$(OUT)\source\screen\candum.obj: $(SRC)\source\screen\candum.c \
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
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\candum.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\candum.c
$(OUT)\source\screen\scrcand.obj: $(SRC)\source\screen\scrcand.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scrcand.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scrcand.c
$(OUT)\source\screen\canfastd.obj: $(SRC)\source\screen\canfastd.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\canfastd.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\canfastd.c
$(OUT)\source\screen\cansvgad.obj: $(SRC)\source\screen\cansvgad.c \
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
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\cansvgad.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\cansvgad.c
$(OUT)\source\screen\scrhws.obj: $(SRC)\source\screen\scrhws.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scrhws.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scrhws.c
$(OUT)\source\screen\canhws.obj: $(SRC)\source\screen\canhws.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\canhws.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\canhws.c
$(OUT)\source\screen\hws.obj: $(SRC)\source\screen\hws.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\pnm.h \
	  $(SRC)\include\dtack\pnm.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\hws.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\hws.c
$(OUT)\source\screen\hwsmsw.obj: $(SRC)\source\screen\hwsmsw.c \
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
	  $(SRC)\include\dtack\label.h \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\hws.h \
	  $(SRC)\include\dtack\hws.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\hwsmsw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\hwsmsw.c
$(OUT)\source\screen\scrhwsd.obj: $(SRC)\source\screen\scrhwsd.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\scr.h \
	  $(SRC)\include\dtack\scr.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\scrhwsd.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\scrhwsd.c
$(OUT)\source\screen\canhwsd.obj: $(SRC)\source\screen\canhwsd.c \
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
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\keymap.h \
	  $(SRC)\include\dtack\keymap.p \
	  $(SRC)\include\dtack\keybd.h \
	  $(SRC)\include\dtack\keybd.p \
	  $(SRC)\include\dtack\svc.h \
	  $(SRC)\include\dtack\svc.p \
	  $(SRC)\include\dtack\canvas.h \
	  $(SRC)\include\dtack\canvas.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\screen\canhwsd.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\screen \
	  -I$(SRC)\include \
	  $(SRC)\source\screen\canhwsd.c


