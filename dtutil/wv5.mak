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
	  $(OUT)\static\dtutil.exe

shexes: \
	  $(OUT)\dynamic\dtutil.exe

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
	-del $(OUT)\dynamic\dtutil.exe
	-del $(OUT)\static\dtutil.exe
	-del $(OUT)\dtutil\*.obj
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



$(OUT)\static\dtutil.exe: \
	  $(OUT)\dtutil\dtutil.obj \
	  
	link /nologo /OUT:$(OUT)\static\dtutil.exe @<<
	  /MAP:$(OUT)\static\dtutil.map
	  /PDB:$(OUT)\static\dtutil.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\dtutil\dtutil.obj
	
	 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtutil.res
<<
$(OUT)\dynamic\dtutil.exe: \
	  $(OUT)\dtutil\dtutil.obj
	link /nologo /OUT:$(OUT)\dynamic\dtutil.exe @<<
	  /MAP:$(OUT)\dynamic\dtutil.map 
	  /PDB:$(OUT)\dynamic\dtutil.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\dtutil\dtutil.obj
	    
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtutil.lib
	del $(OUT)\dynamic\dtutil.exp







#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\dtutil\dtutil.obj: $(SRC)\dtutil\dtutil.c
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\dtutil\dtutil.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\dtutil \
	  -I$(SRC)\include \
	  $(SRC)\dtutil\dtutil.c



# /-----------------------------
# to make this a console mode program
# 1. remove \Gz from compile line
# 2. change subsystem:windows to subsystem:console in link block
# /-----------------------------

subdir_dtutil: default
