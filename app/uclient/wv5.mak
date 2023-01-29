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
	  $(OUT)\static\uclient.exe \
	  $(OUT)\static\userver.exe

shexes: \
	  $(OUT)\dynamic\uclient.exe

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
	-del $(OUT)\dynamic\uclient.exe
	-del $(OUT)\static\uclient.exe
	-del $(OUT)\app\uclient\*.obj
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



$(OUT)\static\uclient.exe: \
	  $(OUT)\app\uclient\uclient.obj \
	  $(OUT)\lib\dtdg.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\uclient.exe @<<
	  /MAP:$(OUT)\static\uclient.map
	  /PDB:$(OUT)\static\uclient.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\app\uclient\uclient.obj
	$(OUT)\lib\dtdg.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\uclient.res
<<
$(OUT)\dynamic\uclient.exe: \
	  $(OUT)\app\uclient\uclient.obj \
	  $(OUT)\libdll\dtdg.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\uclient.exe @<<
	  /MAP:$(OUT)\dynamic\uclient.map 
	  /PDB:$(OUT)\dynamic\uclient.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\app\uclient\uclient.obj
	  $(OUT)\libdll\dtdg.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\uclient.lib
	del $(OUT)\dynamic\uclient.exp





#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\app\uclient\uclient.obj: $(SRC)\app\uclient\uclient.c
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\app\uclient\uclient.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\uclient \
	  -I$(SRC)\include \
	  $(SRC)\app\uclient\uclient.c



#-------------------------------------------------------------------------
# exes



$(OUT)\static\userver.exe: \
	  $(OUT)\app\uclient\userver.obj \
	  $(OUT)\lib\dtdg.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\userver.exe @<<
	  /MAP:$(OUT)\static\userver.map
	  /PDB:$(OUT)\static\userver.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\app\uclient\userver.obj
	$(OUT)\lib\dtdg.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\uclient.res
<<
$(OUT)\dynamic\userver.exe: \
	  $(OUT)\app\uclient\userver.obj \
	  $(OUT)\libdll\dtdg.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\userver.exe @<<
	  /MAP:$(OUT)\dynamic\userver.map 
	  /PDB:$(OUT)\dynamic\userver.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\app\uclient\userver.obj
	  $(OUT)\libdll\dtdg.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\userver.lib
	del $(OUT)\dynamic\userver.exp





#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\app\uclient\userver.obj: $(SRC)\app\uclient\userver.c
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\app\uclient\userver.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\app\uclient \
	  -I$(SRC)\include \
	  $(SRC)\app\uclient\userver.c


