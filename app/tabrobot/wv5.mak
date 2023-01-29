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
	  $(OUT)\static\tabrobot.exe \
	  $(OUT)\static\tabptz.exe \
	  $(OUT)\static\tabroam.exe \
	  $(OUT)\static\ricodog.exe

shexes: \
	  $(OUT)\dynamic\tabrobot.exe \
	  $(OUT)\dynamic\tabptz.exe \
	  $(OUT)\dynamic\tabroam.exe \
	  $(OUT)\dynamic\ricodog.exe

scripts: 

libs: \
	  $(OUT)\lib\dtptz.lib \
	  $(OUT)\lib\dtser.lib \
	  $(OUT)\lib\tabrobox.lib

shlibs: \
	  $(OUT)\dynamic\dtptz.dll \
	  $(OUT)\dynamic\dtser.dll \
	  $(OUT)\dynamic\tabrobox.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\tabrobot.exe
	-del $(OUT)\dynamic\tabptz.exe
	-del $(OUT)\dynamic\tabroam.exe
	-del $(OUT)\dynamic\ricodog.exe
	-del $(OUT)\static\tabrobot.exe
	-del $(OUT)\static\tabptz.exe
	-del $(OUT)\static\tabroam.exe
	-del $(OUT)\static\ricodog.exe
	-del $(OUT)\lib\dtptz.lib
	-del $(OUT)\lib\dtser.lib
	-del $(OUT)\lib\tabrobox.lib
	-del $(OUT)\dynamic\dtptz.dll
	-del $(OUT)\dynamic\dtser.dll
	-del $(OUT)\dynamic\tabrobox.dll
	-del $(OUT)\app\tabrobot\*.obj
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



$(OUT)\static\tabrobot.exe: \
	  $(OUT)\app\tabrobot\tabrobot.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\lib\tabrobox.lib \
	  $(OUT)\lib\dtptz.lib \
	  $(OUT)\lib\dtser.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\tabrobot.exe @<<
	  /MAP:$(OUT)\static\tabrobot.map
	  /PDB:$(OUT)\static\tabrobot.pdb
 /DEBUG /STACK:64000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\tabrobot\tabrobot.obj
	$(OUT)\app\tabrobot\log.obj
	$(OUT)\lib\tabrobox.lib
	$(OUT)\lib\dtptz.lib
	$(OUT)\lib\dtser.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\tabrobot.res
<<
$(OUT)\dynamic\tabrobot.exe: \
	  $(OUT)\app\tabrobot\tabrobot.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\libdll\tabrobox.lib \
	  $(OUT)\libdll\dtptz.lib \
	  $(OUT)\libdll\dtser.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\tabrobot.exe @<<
	  /MAP:$(OUT)\dynamic\tabrobot.map 
	  /PDB:$(OUT)\dynamic\tabrobot.pdb 

	  /DEBUG
	  /STACK:64000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\tabrobot\tabrobot.obj
	  $(OUT)\app\tabrobot\log.obj
	  $(OUT)\libdll\tabrobox.lib
	  $(OUT)\libdll\dtptz.lib
	  $(OUT)\libdll\dtser.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\tabrobot.lib
	del $(OUT)\dynamic\tabrobot.exp



$(OUT)\static\tabptz.exe: \
	  $(OUT)\app\tabrobot\tabptz.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\lib\dtptz.lib \
	  $(OUT)\lib\dtser.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\tabptz.exe @<<
	  /MAP:$(OUT)\static\tabptz.map
	  /PDB:$(OUT)\static\tabptz.pdb
 /DEBUG /STACK:64000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\tabrobot\tabptz.obj
	$(OUT)\app\tabrobot\log.obj
	$(OUT)\lib\dtptz.lib
	$(OUT)\lib\dtser.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\tabptz.res
<<
$(OUT)\dynamic\tabptz.exe: \
	  $(OUT)\app\tabrobot\tabptz.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\libdll\dtptz.lib \
	  $(OUT)\libdll\dtser.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\tabptz.exe @<<
	  /MAP:$(OUT)\dynamic\tabptz.map 
	  /PDB:$(OUT)\dynamic\tabptz.pdb 

	  /DEBUG
	  /STACK:64000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\tabrobot\tabptz.obj
	  $(OUT)\app\tabrobot\log.obj
	  $(OUT)\libdll\dtptz.lib
	  $(OUT)\libdll\dtser.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\tabptz.lib
	del $(OUT)\dynamic\tabptz.exp



$(OUT)\static\tabroam.exe: \
	  $(OUT)\app\tabrobot\tabroam.obj \
	  $(OUT)\app\tabrobot\roam.obj \
	  $(OUT)\app\tabrobot\client.obj \
	  $(OUT)\app\tabrobot\preset.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\app\tabrobot\rev.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\tabroam.exe @<<
	  /MAP:$(OUT)\static\tabroam.map
	  /PDB:$(OUT)\static\tabroam.pdb
 /DEBUG /STACK:64000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\tabrobot\tabroam.obj
	$(OUT)\app\tabrobot\roam.obj
	$(OUT)\app\tabrobot\client.obj
	$(OUT)\app\tabrobot\preset.obj
	$(OUT)\app\tabrobot\log.obj
	$(OUT)\app\tabrobot\rev.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\tabroam.res
<<
$(OUT)\dynamic\tabroam.exe: \
	  $(OUT)\app\tabrobot\tabroam.obj \
	  $(OUT)\app\tabrobot\roam.obj \
	  $(OUT)\app\tabrobot\client.obj \
	  $(OUT)\app\tabrobot\preset.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\app\tabrobot\rev.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\tabroam.exe @<<
	  /MAP:$(OUT)\dynamic\tabroam.map 
	  /PDB:$(OUT)\dynamic\tabroam.pdb 

	  /DEBUG
	  /STACK:64000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\tabrobot\tabroam.obj
	  $(OUT)\app\tabrobot\roam.obj
	  $(OUT)\app\tabrobot\client.obj
	  $(OUT)\app\tabrobot\preset.obj
	  $(OUT)\app\tabrobot\log.obj
	  $(OUT)\app\tabrobot\rev.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_FLEXLM_LOAD)
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\tabroam.lib
	del $(OUT)\dynamic\tabroam.exp



$(OUT)\static\ricodog.exe: \
	  $(OUT)\app\tabrobot\ricodog.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\lib\dtser.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\ricodog.exe @<<
	  /MAP:$(OUT)\static\ricodog.map
	  /PDB:$(OUT)\static\ricodog.pdb
 /DEBUG /STACK:64000
          /NODEFAULTLIB:msvcrt
	$(OUT)\app\tabrobot\ricodog.obj
	$(OUT)\app\tabrobot\log.obj
	$(OUT)\lib\dtser.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\ricodog.res
<<
$(OUT)\dynamic\ricodog.exe: \
	  $(OUT)\app\tabrobot\ricodog.obj \
	  $(OUT)\app\tabrobot\log.obj \
	  $(OUT)\libdll\dtser.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\ricodog.exe @<<
	  /MAP:$(OUT)\dynamic\ricodog.map 
	  /PDB:$(OUT)\dynamic\ricodog.pdb 

	  /DEBUG
	  /STACK:64000
          /NODEFAULTLIB:msvcrt
	  $(OUT)\app\tabrobot\ricodog.obj
	  $(OUT)\app\tabrobot\log.obj
	  $(OUT)\libdll\dtser.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\ricodog.lib
	del $(OUT)\dynamic\ricodog.exp



#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dtptz.lib: \
	  $(OUT)\app\tabrobot\ptz.obj \
	  $(OUT)\app\tabrobot\ptzpela.obj \
	  $(OUT)\app\tabrobot\ptzipc.obj \
	  $(OUT)\app\tabrobot\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtptz.lib @<<
	  $(OUT)\app\tabrobot\ptz.obj
	  $(OUT)\app\tabrobot\ptzpela.obj
	  $(OUT)\app\tabrobot\ptzipc.obj
	  $(OUT)\app\tabrobot\libmain.obj
<<
	
$(OUT)\dynamic\dtptz.dll \
$(OUT)\libdll\dtptz.lib: \
	  $(OUT)\app\tabrobot\ptz.obj \
	  $(OUT)\app\tabrobot\ptzpela.obj \
	  $(OUT)\app\tabrobot\ptzipc.obj \
	  $(OUT)\app\tabrobot\libmain.obj \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\dtptz.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtptz.lib
	  $(OUT)\app\tabrobot\ptz.obj
	  $(OUT)\app\tabrobot\ptzpela.obj
	  $(OUT)\app\tabrobot\ptzipc.obj
	  $(OUT)\app\tabrobot\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtser.lib: \
	  $(OUT)\app\tabrobot\serial.obj \
	  $(OUT)\app\tabrobot\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtser.lib @<<
	  $(OUT)\app\tabrobot\serial.obj
	  $(OUT)\app\tabrobot\libmain.obj
<<
	
$(OUT)\dynamic\dtser.dll \
$(OUT)\libdll\dtser.lib: \
	  $(OUT)\app\tabrobot\serial.obj \
	  $(OUT)\app\tabrobot\libmain.obj \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\dtser.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtser.lib
	  $(OUT)\app\tabrobot\serial.obj
	  $(OUT)\app\tabrobot\libmain.obj
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\tabrobox.lib: \
	  $(OUT)\app\tabrobot\rev.obj \
	  $(OUT)\app\tabrobot\client.obj \
	  $(OUT)\app\tabrobot\gui_qnx.obj \
	  $(OUT)\app\tabrobot\preset.obj \
	  $(OUT)\app\tabrobot\server.obj \
	  $(OUT)\app\tabrobot\libmain.obj
	lib /nologo /out:$(OUT)\lib\tabrobox.lib @<<
	  $(OUT)\app\tabrobot\rev.obj
	  $(OUT)\app\tabrobot\client.obj
	  $(OUT)\app\tabrobot\gui_qnx.obj
	  $(OUT)\app\tabrobot\preset.obj
	  $(OUT)\app\tabrobot\server.obj
	  $(OUT)\app\tabrobot\libmain.obj
<<
	
$(OUT)\dynamic\tabrobox.dll \
$(OUT)\libdll\tabrobox.lib: \
	  $(OUT)\app\tabrobot\rev.obj \
	  $(OUT)\app\tabrobot\client.obj \
	  $(OUT)\app\tabrobot\gui_qnx.obj \
	  $(OUT)\app\tabrobot\preset.obj \
	  $(OUT)\app\tabrobot\server.obj \
	  $(OUT)\app\tabrobot\libmain.obj \
	  $(OUT)\libdll\dtptz.lib \
	  $(OUT)\libdll\dtlow.lib
	link /nologo /out:$(OUT)\dynamic\tabrobox.dll @<<!
	  /dll /implib:$(OUT)\libdll\tabrobox.lib
	  $(OUT)\app\tabrobot\rev.obj
	  $(OUT)\app\tabrobot\client.obj
	  $(OUT)\app\tabrobot\gui_qnx.obj
	  $(OUT)\app\tabrobot\preset.obj
	  $(OUT)\app\tabrobot\server.obj
	  $(OUT)\app\tabrobot\libmain.obj
	  $(OUT)\libdll\dtptz.lib
	  $(OUT)\libdll\dtlow.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\app\tabrobot\gui_qnx.obj: $(SRC)\app\tabrobot\gui_qnx.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\app\tabrobot\operate.i
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\gui_qnx.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\gui_qnx.c
$(OUT)\app\tabrobot\ptzpela.obj: $(SRC)\app\tabrobot\ptzpela.c \
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
	  $(SRC)\app\tabrobot\dtptz.h \
	  $(SRC)\app\tabrobot\dtptz.p \
	  $(SRC)\app\tabrobot\dtserial.h \
	  $(SRC)\app\tabrobot\dtserial.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\ptzpela.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_PTZ_PELA=DT_1 \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\ptzpela.c
$(OUT)\app\tabrobot\ptz.obj: $(SRC)\app\tabrobot\ptz.c \
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
	  $(SRC)\app\tabrobot\dtptz.h \
	  $(SRC)\app\tabrobot\dtptz.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\ptz.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\ptz.c
$(OUT)\app\tabrobot\ptzipc.obj: $(SRC)\app\tabrobot\ptzipc.c \
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
	  $(SRC)\app\tabrobot\dtptz.h \
	  $(SRC)\app\tabrobot\dtptz.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\ptzipc.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\ptzipc.c
$(OUT)\app\tabrobot\libmain.obj: $(SRC)\app\tabrobot\libmain.c \
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
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\libmain.c
$(OUT)\app\tabrobot\serial.obj: $(SRC)\app\tabrobot\serial.c \
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
	  $(SRC)\app\tabrobot\dtserial.h \
	  $(SRC)\app\tabrobot\dtserial.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\serial.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\serial.c
$(OUT)\app\tabrobot\rev.obj: $(SRC)\app\tabrobot\rev.c
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\rev.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\rev.c
$(OUT)\app\tabrobot\client.obj: $(SRC)\app\tabrobot\client.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\client.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\client.c
$(OUT)\app\tabrobot\preset.obj: $(SRC)\app\tabrobot\preset.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\preset.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\preset.c
$(OUT)\app\tabrobot\server.obj: $(SRC)\app\tabrobot\server.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\app\tabrobot\dtptz.h \
	  $(SRC)\app\tabrobot\dtptz.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\server.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\server.c
$(OUT)\app\tabrobot\tabrobot.obj: $(SRC)\app\tabrobot\tabrobot.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\app\tabrobot\dtptz.h \
	  $(SRC)\app\tabrobot\dtptz.p \
	  $(SRC)\include\dtack\licens.h \
	  $(SRC)\app\tabrobot\tabrobot.use
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\tabrobot.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\tabrobot.c
$(OUT)\app\tabrobot\log.obj: $(SRC)\app\tabrobot\log.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\include\dtack\label.h
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\log.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\log.c
$(OUT)\app\tabrobot\tabptz.obj: $(SRC)\app\tabrobot\tabptz.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\app\tabrobot\dtptz.h \
	  $(SRC)\app\tabrobot\dtptz.p \
	  $(SRC)\include\dtack\licens.h \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\app\tabrobot\tabptz.use
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\tabptz.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\tabptz.c
$(OUT)\app\tabrobot\tabroam.obj: $(SRC)\app\tabrobot\tabroam.c
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\tabroam.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\tabroam.c
$(OUT)\app\tabrobot\roam.obj: $(SRC)\app\tabrobot\roam.c
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\roam.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\roam.c
$(OUT)\app\tabrobot\ricodog.obj: $(SRC)\app\tabrobot\ricodog.c \
	  $(SRC)\app\tabrobot\tabrobot.h \
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
	  $(SRC)\app\tabrobot\tabrobot.p \
	  $(SRC)\app\tabrobot\dtserial.h \
	  $(SRC)\app\tabrobot\dtserial.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\app\tabrobot\ricodog.use
	cl /nologo /c /Gf /J /GB /MD /Gz /DDBG=1 /DDEBUG /D_DEBUG \
	   /Fo$(OUT)\app\tabrobot\ricodog.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\tabrobot \
	  -I$(SRC)\include \
	  $(SRC)\app\tabrobot\ricodog.c


